# Feature: Console BMI and Basal Metabolic Rate Estimator

## Feature Description

Add an interactive, dependency-free C++ console application that collects height, weight, age, and the profile required by the chosen metabolic-rate equation. It reports BMI in kg/m² and an estimated basal metabolic rate (BMR) in kcal/day. The output should identify both values as estimates and avoid implying that they are a diagnosis.

## User Story

As a person using a console application,
I want to enter my measurements and age,
so that I can see my BMI and an estimated baseline metabolic rate.

## Problem Statement

The repository currently has an empty `main()` function. It needs an input flow, validated calculations, and clearly labeled results for BMI and estimated BMR.

## Solution Statement

Implement small calculation functions separately from console input/output. Proposed calculation defaults are metric input (height in centimeters, weight in kilograms) and the Mifflin–St Jeor equation. BMI is `weight_kg / (height_m * height_m)`. Mifflin–St Jeor estimates resting energy expenditure using age, height, weight, and a sex-specific equation constant. The user-facing prompt and available equation profiles must be agreed before implementation.

No third-party library is needed. The application and tests should preserve the repository’s current root-level build pattern.

## Relevant Files

- `README.md` — Documents the root C++ application and container workflow.
- `main.cpp` — Empty entry point where the interactive flow will be connected.
- `test_runner.sh` — Compiles `*.cpp` from the repository root and launches the resulting app. Avoid adding a second root-level `main()`.
- `tests/README.md` — Establishes that `tests/` is intended for test code; no test framework is documented.
- `specs/README.md` — Describes the purpose of this directory.

### New Files

- `bmi_bmr.h` — Declarations for calculation functions, with units clear in names or comments.
- `bmi_bmr.cpp` — BMI and BMR calculation implementations, independent of console I/O.
- `tests/test_calculations.cpp` — A small calculation test executable, compiled explicitly with `bmi_bmr.cpp` and without `main.cpp`.
- `tests/test_console.sh` — Scripted integration checks that build the app and exercise valid, invalid, and closed-input sessions.

## Implementation Plan

### Phase 1: Foundation

- Resolve input units, BMR equation profile choices and labels, and accepted age/input ranges.
- Define calculation function signatures and their input contracts.
- Keep test sources beneath `tests/` so the root `g++ *.cpp` command does not include another `main()`.

### Phase 2: Core Implementation

- Implement BMI and Mifflin–St Jeor calculations as independent functions.
- Implement console prompts, parsing, validation, and controlled handling of invalid input and EOF.
- Display values with units, appropriate precision, and a short estimation note.

### Phase 3: Integration

- Connect calculations to `main.cpp` and preserve compatibility with `test_runner.sh`.
- Add direct calculation checks and scripted console checks using explicit compile commands.
- Verify the normal root build and manual interactive flow.

## Step by Step Tasks

### 1. Set input and equation conventions

- Accept metric only: height 100–250 cm, weight 20–500 kg, and whole-number age 19–78.
- Use the Mifflin–St Jeor male/female equation profiles and explain that this is a formula input, not gender identity.
- Reprompt after invalid entries; exit cleanly without results on EOF.
- The age range matches the original Mifflin–St Jeor study sample. The height and weight bounds are broad input guardrails for v1.

### 2. Implement calculation functions

- Add `bmi_bmr.h` and `bmi_bmr.cpp` with small functions that do not read from or write to the console.
- Use centimeters, kilograms, years, and kcal/day consistently.
- BMI formula: `weight_kg / (height_m * height_m)`.
- If Mifflin–St Jeor is confirmed, use the chosen profile constant in `10 * weight_kg + 6.25 * height_cm - 5 * age_years + constant` (commonly `+5` and `-161` for the male and female equation variants respectively).

### 3. Add calculation tests

- Add `tests/test_calculations.cpp` with known BMI and BMR examples for every supported equation profile.
- Cover invalid values according to the function contracts, including zero/negative height and weight and unsupported profile values if represented programmatically.
- Compile tests explicitly, excluding the application's `main.cpp`.

### 4. Implement console input and output

- Update `main.cpp` to prompt for each agreed input and call the calculation functions.
- Reject malformed, non-finite, non-positive, or out-of-range values according to the decisions in Task 1.
- Handle invalid profile selections and EOF cleanly; do not loop indefinitely or calculate from stale/partial input.
- Print BMI in kg/m² and estimated BMR in kcal/day with clear labels and suitable rounding.

### 5. Add console integration checks

- Add `tests/test_console.sh` to compile and run representative valid and invalid sessions through piped input.
- Check successful results, validation/error behavior, and EOF at input prompts without relying on fragile exact floating-point formatting.
- Keep the existing root-level application build behavior intact.

### 6. Run validation commands

- Run the calculation test executable and console integration script.
- Build/run the root app with the existing `test_runner.sh` workflow and verify one manual interactive session.

## Testing Strategy

### Unit Tests

- BMI matches hand-calculated values for representative metric measurements.
- Each supported Mifflin–St Jeor profile matches hand-calculated values.
- Invalid inputs are handled according to the documented calculation contracts.

### Edge Cases

- Nonnumeric and malformed input.
- Zero, negative, or non-finite height and weight.
- Age at and outside the accepted boundaries.
- Invalid profile selection.
- EOF at every prompt.
- Values outside any chosen plausible input range.

## Acceptance Criteria

- The program accepts the agreed inputs and computes BMI and estimated BMR using the agreed formulas and units.
- Results are labeled with kg/m² and kcal/day and BMR is explicitly described as an estimate.
- Invalid and incomplete input produces clear behavior without a hang or accidental calculation.
- Calculation and console checks cover the supported input and equation profiles.
- The existing root-level compile/run workflow continues to work without multiple-definition errors.

## Validation Commands

- `g++ -std=c++17 -Wall -Wextra -pedantic tests/test_calculations.cpp bmi_bmr.cpp -o /tmp/test_calculations && /tmp/test_calculations`
- `bash tests/test_console.sh`
- `bash test_runner.sh` — use an interactive terminal to verify the established workflow.

## Risks and Mitigations

| Risk | Impact | Mitigation |
|---|---|---|
| Ambiguous use of “gender” for a sex-specific equation input | The prompt can misrepresent what the equation asks for or omit a needed profile | Confirm the wording and supported profiles before implementation; explain the field is used by the selected equation |
| Root wildcard compilation picks up test `main()` | The app fails to link | Keep C++ test sources under `tests/` and compile them explicitly |
| Failed reads or EOF cause repeated prompts | The console program can hang or emit repeated errors | Treat stream failure/EOF as a clean exit and include it in integration checks |
| Formula estimate is interpreted as a measured or diagnostic value | Users may over-rely on the result | Label results as estimates and avoid adding unsupported interpretation |

## Decisions

- v1 accepts metric inputs only: height 100–250 cm, weight 20–500 kg, and age 19–78 years.
- v1 uses the Mifflin–St Jeor male/female equation variants. The console describes this as an equation profile, not gender identity.
- Invalid entries reprompt; EOF exits without calculating results.
- Calculation and console tests are compiled explicitly so they do not enter the root `*.cpp` application build.

## Review Notes

- The plan reviewer confirmed the current project layout supports a dependency-free implementation.
- Calculation tests must be kept out of the root wildcard build because that build compiles every root `.cpp` file.
- Tests should exercise EOF to prevent a common interactive-loop failure.
- BMI is calculated as weight in kilograms divided by height in meters squared, per [CDC guidance](https://www.cdc.gov/bmi/about/index.html). The Mifflin–St Jeor equation was published as a predictive equation for resting energy expenditure in healthy individuals; see the [original study record](https://pubmed.ncbi.nlm.nih.gov/2305711/?dopt=Abstract).
