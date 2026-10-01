#include "../bmi_bmr.h"

#include <cmath>
#include <exception>
#include <iostream>

namespace {

bool near(double actual, double expected) {
  return std::abs(actual - expected) < 0.01;
}

int failures = 0;

void expect_near(const char *name, double actual, double expected) {
  if (!near(actual, expected)) {
    std::cerr << name << ": expected " << expected << ", got " << actual << '\n';
    ++failures;
  }
}

template <typename Function>
void expect_invalid(const char *name, Function function) {
  try {
    function();
    std::cerr << name << ": expected std::invalid_argument\n";
    ++failures;
  } catch (const std::invalid_argument &) {
  } catch (const std::exception &error) {
    std::cerr << name << ": unexpected exception: " << error.what() << '\n';
    ++failures;
  }
}

} // namespace

int main() {
  expect_near("BMI", bmi_bmr::calculate_bmi(180.0, 75.0), 75.0 / (1.8 * 1.8));
  expect_near("male BMR", bmi_bmr::calculate_bmr(180.0, 75.0, 30,
                                                  bmi_bmr::EquationProfile::Male),
              1730.0);
  expect_near("female BMR", bmi_bmr::calculate_bmr(180.0, 75.0, 30,
                                                    bmi_bmr::EquationProfile::Female),
              1564.0);

  expect_invalid("zero BMI height", [] { bmi_bmr::calculate_bmi(0.0, 75.0); });
  expect_invalid("negative BMI weight", [] { bmi_bmr::calculate_bmi(180.0, -1.0); });
  expect_invalid("non-finite BMR height", [] {
    bmi_bmr::calculate_bmr(INFINITY, 75.0, 30, bmi_bmr::EquationProfile::Male);
  });
  expect_invalid("unrepresentable BMI", [] {
    bmi_bmr::calculate_bmi(1.0e200, 75.0);
  });
  expect_invalid("invalid BMR age", [] {
    bmi_bmr::calculate_bmr(180.0, 75.0, 0, bmi_bmr::EquationProfile::Male);
  });
  expect_invalid("nonpositive BMR result", [] {
    bmi_bmr::calculate_bmr(0.1, 1.0, 78, bmi_bmr::EquationProfile::Female);
  });
  expect_invalid("invalid BMR profile", [] {
    bmi_bmr::calculate_bmr(180.0, 75.0, 30,
                           static_cast<bmi_bmr::EquationProfile>(99));
  });

  if (failures != 0) {
    return 1;
  }
  std::cout << "All calculation checks passed.\n";
  return 0;
}
