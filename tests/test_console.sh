#!/usr/bin/env bash
set -euo pipefail

repo_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
build_dir="$(mktemp -d)"
trap 'rm -rf "$build_dir"' EXIT

g++ -std=c++17 -Wall -Wextra -pedantic \
  "$repo_dir/main.cpp" "$repo_dir/bmi_bmr.cpp" -o "$build_dir/app"

expect_contains() {
  local output="$1"
  local expected="$2"
  if [[ "$output" != *"$expected"* ]]; then
    printf 'Expected console output to contain: %s\nActual output:\n%s\n' \
      "$expected" "$output" >&2
    exit 1
  fi
}

male_output="$(printf '180\n75\n30\nM\n' | "$build_dir/app")"
expect_contains "$male_output" "BMI: 23.15 kg/m^2"
expect_contains "$male_output" "Estimated BMR: 1730 kcal/day"

female_output="$(printf '180\n75\n30\nf\n' | "$build_dir/app")"
expect_contains "$female_output" "Estimated BMR: 1564 kcal/day"

boundary_output="$(printf '180\n75\n78\nF\n' | "$build_dir/app")"
expect_contains "$boundary_output" "Estimated BMR: 1324 kcal/day"

lower_bound_output="$(printf '100\n20\n19\nf\n' | "$build_dir/app")"
expect_contains "$lower_bound_output" "BMI: 20.00 kg/m^2"
expect_contains "$lower_bound_output" "Estimated BMR: 569 kcal/day"

upper_bound_output="$(printf '250\n500\n78\nM\n' | "$build_dir/app")"
expect_contains "$upper_bound_output" "BMI: 80.00 kg/m^2"
expect_contains "$upper_bound_output" "Estimated BMR: 6178 kcal/day"

invalid_output="$(printf 'abc\nnan\n180\ninf\n0\n75\n17\n30\nx\nF\n' | "$build_dir/app")"
expect_contains "$invalid_output" "Enter a finite height in cm from 100 to 250."
expect_contains "$invalid_output" "Enter a finite weight in kg from 20 to 500."
expect_contains "$invalid_output" "Enter a whole-number age from 19 to 78."
expect_contains "$invalid_output" "Enter M or F for the equation profile."
expect_contains "$invalid_output" "Estimated BMR: 1564 kcal/day"

eof_inputs=("" $'180\n' $'180\n75\n' $'180\n75\n30\n')
for eof_input in "${eof_inputs[@]}"; do
  eof_output="$(printf '%s' "$eof_input" | "$build_dir/app")"
  expect_contains "$eof_output" "Input ended before all values were entered. No results calculated."
  if [[ "$eof_output" == *"BMI:"* || "$eof_output" == *"Estimated BMR:"* ]]; then
    printf 'EOF session unexpectedly produced calculation results.\n' >&2
    exit 1
  fi
done

echo "All console checks passed."
