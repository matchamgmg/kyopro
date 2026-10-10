# AHC072 solver case comparison

`compare_case_strengths.py` compares these two solver families on a directory of inputs.

- `ahc072_pooled_a025_compare.cpp`: pooled historical repair + weak 2-step lookahead (`alpha=1/4`)
- `ahc072_fill8_mixed_compare.cpp`: fill8/flushX + color-fixed/interleave + small-color mixed transport

## Run

```bash
python3 compare_case_strengths.py \
  --inputs ./in \
  --pattern '*.txt' \
  --pooled-source ./ahc072_pooled_a025_compare.cpp \
  --structured-source ./ahc072_fill8_mixed_compare.cpp
```

The script compiles both solvers and runs them **sequentially** on every case so their internal ~1.9 s time limits do not compete for CPU.

Outputs:

- `case_comparison.csv`: per-case input features, action counts, winner, runtime, and solver logs parsed into columns.
- `case_feature_summary.csv`: input-feature means for pooled-win vs structured-win cases, sorted by standardized difference.
- `case_compare_logs/`: stderr for both solvers on every case.

Useful columns include wall density, color-count distribution, number of small colors/slimes, slime-to-nest distances, Greedy-Steiner edge counts, same-color and cross-color shortest-path overlap, corridor degree statistics, straight-run statistics, pooled event/beam statistics, and fill8/flushX strategy statistics.

`pooled_actions` and `structured_actions` are counted from output action lines. This is appropriate when both solvers are known to produce complete legal solutions. If you want strict validity/score checking, run the outputs through the official tester as a separate step.
