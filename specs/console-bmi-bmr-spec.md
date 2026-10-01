# Spec: Console BMI and Basal Metabolic Rate Estimator

## Objective

Build a small, interactive C++ console application that asks a user for height, weight, age, and the equation profile required for an estimated metabolic-rate calculation. The program reports BMI and an estimated baseline metabolic rate in clearly labeled units.

The application is intended for basic educational/personal use. It must not present BMI as a diagnosis or estimated BMR as a measured metabolic rate.

### User Story

As a person using a console application, I want to enter my measurements and age so that I can see my BMI and estimated baseline metabolic rate.

### Input and calculation decisions

1. v1 accepts metric measurements only: height from 100 to 250 cm and weight from 20 to 500 kg.
2. Age is a whole number from 19 to 78 years, the age range represented in the original Mifflin–St Jeor study sample.
3. BMI uses `weight_kg / height_m²` and is reported in kg/m².
4. BMR uses Mifflin–St Jeor with male and female equation variants. The prompt explains that the selected profile is a formula input, not gender identity.
5. Invalid values reprompt; EOF exits cleanly without results.
6. The program has no external dependencies and remains compatible with the existing root-level `g++ *.cpp` application build.

## Tech Stack

- C++ using the compiler available in the project container (`g++`).
- Standard C++ library only; no third-party packages.
- Shell scripts for the existing build/run and proposed scripted console checks.
- Exact language standard is not declared by the repository. The implementation plan proposes C++17 for explicit test compilation; confirm compiler support before relying on C++17-only features.

## Commands

Existing application command, as used by `test_runner.sh`:

```bash
g++ *.cpp -o app
./app
```

Proposed calculation test command after implementation:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic tests/test_calculations.cpp bmi_bmr.cpp -o /tmp/test_calculations
/tmp/test_calculations
```

Proposed console integration check after implementation:

```bash
bash tests/test_console.sh
```

The existing runner compiles and starts an interactive program, so use a terminal with sample input when validating it:

```bash
bash test_runner.sh
```

## Project Structure

```text
main.cpp                       # Console prompts, input flow, and result presentation
bmi_bmr.h                      # Calculation declarations and unit contracts
bmi_bmr.cpp                    # BMI and BMR calculations, independent of console I/O
tests/test_calculations.cpp    # Standalone calculation checks; compile explicitly
tests/test_console.sh          # Piped-input checks of the interactive application
test_runner.sh                 # Existing root wildcard build and interactive launch
specs/console-bmi-bmr-plan.md  # Refined implementation plan
specs/console-bmi-bmr-spec.md  # This feature specification
```

Keep test C++ files under `tests/`: the current application build uses the root glob `*.cpp`, and a second root-level test `main()` would cause a link conflict.

## Code Style

- Keep calculation functions independent from `std::cin` and `std::cout` so they can be tested directly.
- Include units in parameter names or document them at the declaration (for example `height_cm`, `weight_kg`, `age_years`).
- Use descriptive names and standard-library types; avoid adding abstractions or dependencies that are not needed for this small app.
- Parse and validate input before calculating. Handle stream failure and EOF explicitly.
- Keep output labels and units visible. An example of the intended result shape is:

```text
BMI: 22.86 kg/m²
Estimated BMR: 1,650 kcal/day
```

The displayed values above are illustrative only; actual calculations must use the user's inputs and selected profile.

## Testing Strategy

There is no test framework currently documented. Use a small standalone C++ test executable for calculation functions and a shell script for interactive integration checks unless the project establishes a different convention.

### Calculation checks

- Verify BMI against hand-calculated metric examples.
- Verify each supported Mifflin–St Jeor variant against hand-calculated examples.
- Verify calculation contracts for invalid values, including zero/negative measurements and unsupported profile values when represented programmatically.
- Compare numeric results with a tolerance; do not make tests depend on incidental display rounding.

### Console checks

- Feed a valid input session and verify the expected labeled results and units.
- Exercise malformed, non-finite, non-positive, and out-of-range inputs according to the decisions recorded before implementation.
- Exercise unsupported profile choices and EOF at each prompt.
- Confirm invalid input neither uses stale values nor causes an infinite retry loop.
- Validate metric measurements against the v1 bounds and restrict age to 19–78 years.

### Build/integration checks

- Build the application with the root wildcard command used by `test_runner.sh`.
- Compile calculation tests explicitly without `main.cpp`.
- Run the existing interactive runner manually with representative input.

## Boundaries

- **Always:** Keep calculation logic separate from console I/O; validate parsed inputs; label units and estimated values; handle EOF without hanging; keep test entry points under `tests/`; preserve the existing application build workflow.
- **Ask first:** Changing the BMR equation; adding support for additional units or equation profiles beyond the choices approved for v1; adding dependencies; changing the existing runner or project build configuration; presenting BMI categories or health recommendations.
  - **Never:** Present BMI as a diagnosis or estimated BMR as a measured result; calculate from malformed or partially read input; silently treat gender identity as the sex-specific equation input; add a second root-level `main()` that conflicts with the wildcard build.

## Success Criteria

- The program prompts for all user inputs selected by the approved v1 decisions.
- Valid input produces BMI in kg/m² and estimated BMR in kcal/day using the approved formulas.
- Results are labeled as estimates and are formatted clearly.
- Invalid or incomplete input produces the approved recovery/error behavior without hanging or producing a result from invalid data.
- Calculation and console checks cover all supported calculation profiles and important invalid-input paths.
- The application still builds with `g++ *.cpp -o app` and the existing runner remains usable.

## Implementation Plan

1. Resolve open input and validation choices.
2. Add the independent BMI/BMR calculation interface and implementation.
3. Add standalone calculation checks.
4. Implement the console prompts, parsing, validation, and output in `main.cpp`.
5. Add scripted console integration checks, including invalid input and EOF.
6. Run the calculation checks, console checks, root build, and an interactive session.

The reviewed task breakdown and file-level details are in [console-bmi-bmr-plan.md](console-bmi-bmr-plan.md).

## Open Questions

No product decisions remain open for the current implementation. The metric input bounds are broad v1 validation guardrails; extending units, equation profiles, or supported age ranges would require updating this spec.

The BMI formula is consistent with [CDC guidance](https://www.cdc.gov/bmi/about/index.html). The Mifflin–St Jeor equation was published as a predictive equation for resting energy expenditure in healthy individuals; see the [original study record](https://pubmed.ncbi.nlm.nih.gov/2305711/?dopt=Abstract).
