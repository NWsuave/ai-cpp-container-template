#ifndef BMI_BMR_H
#define BMI_BMR_H

namespace bmi_bmr {

enum class EquationProfile { Male, Female };

// Height is in centimeters and weight is in kilograms. The result is kg/m^2.
double calculate_bmi(double height_cm, double weight_kg);

// Uses Mifflin-St Jeor. Returns estimated resting energy expenditure in kcal/day.
double calculate_bmr(double height_cm, double weight_kg, int age_years,
                     EquationProfile profile);

} // namespace bmi_bmr

#endif
