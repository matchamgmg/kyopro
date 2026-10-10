#!/usr/bin/env python3
from __future__ import annotations

import argparse
import csv
import math
import os
import re
import shlex
import statistics
import subprocess
import sys
import time
from collections import deque
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable

DX = (-1, 1, 0, 0)
DY = (0, 0, -1, 1)
SMALL_COLOR_MAX_SLIMES = 7


@dataclass
class Problem:
    n: int
    k: int
    grid: list[str]
    nests: list[int]
    terminals: list[list[int]]
    passable: list[bool]

    @property
    def v(self) -> int:
        return self.n * self.n


def parse_problem(path: Path) -> Problem:
    lines = [line.rstrip("\n") for line in path.read_text(encoding="utf-8").splitlines() if line.strip()]
    if not lines:
        raise ValueError("empty input")
    n, k = map(int, lines[0].split())
    if len(lines) < n + 1:
        raise ValueError(f"expected {n} grid rows, got {len(lines) - 1}")
    grid = lines[1 : n + 1]
    if any(len(row) != n for row in grid):
        raise ValueError("grid width mismatch")

    nests = [-1] * k
    terminals: list[list[int]] = [[] for _ in range(k)]
    passable = [False] * (n * n)

    for x in range(n):
        for y in range(n):
            v = x * n + y
            c = grid[x][y]
            passable[v] = c != "#"
            if "A" <= c <= "L":
                color = ord(c) - ord("A")
                if color < k:
                    nests[color] = v
            elif "a" <= c <= "l":
                color = ord(c) - ord("a")
                if color < k:
                    terminals[color].append(v)

    for c in range(k):
        if nests[c] < 0:
            raise ValueError(f"nest for color {c} not found")

    return Problem(n, k, grid, nests, terminals, passable)


def neighbors(p: Problem, v: int) -> Iterable[int]:
    x, y = divmod(v, p.n)
    for dx, dy in zip(DX, DY):
        nx, ny = x + dx, y + dy
        if 0 <= nx < p.n and 0 <= ny < p.n:
            nv = nx * p.n + ny
            if p.passable[nv]:
                yield nv


def bfs(p: Problem, source: int) -> list[int]:
    inf = 10**9
    dist = [inf] * p.v
    dist[source] = 0
    q = deque([source])
    while q:
        v = q.popleft()
        nd = dist[v] + 1
        for nv in neighbors(p, v):
            if dist[nv] != inf:
                continue
            dist[nv] = nd
            q.append(nv)
    return dist


def greedy_steiner_edge_count(p: Problem, color: int) -> int:
    terminals = p.terminals[color]
    if not terminals:
        return 0

    in_tree = [False] * p.v
    is_terminal = [False] * p.v
    for v in terminals:
        is_terminal[v] = True
    in_tree[p.nests[color]] = True
    edges: set[tuple[int, int]] = set()

    while any(not in_tree[v] for v in terminals):
        prev = [-2] * p.v
        q: deque[int] = deque()
        # C++版と同じく cell id 昇順で multi-source を投入。
        for v in range(p.v):
            if in_tree[v]:
                prev[v] = -1
                q.append(v)

        target = -1
        while q:
            v = q.popleft()
            if is_terminal[v] and not in_tree[v]:
                target = v
                break
            for nv in neighbors(p, v):
                if prev[nv] != -2:
                    continue
                prev[nv] = v
                q.append(nv)

        if target < 0:
            return 10**9

        cur = target
        while prev[cur] != -1:
            par = prev[cur]
            edges.add((min(cur, par), max(cur, par)))
            in_tree[cur] = True
            in_tree[par] = True
            cur = par

    return len(edges)


def mean_or_zero(xs: list[float | int]) -> float:
    return float(sum(xs) / len(xs)) if xs else 0.0


def pstdev_or_zero(xs: list[float | int]) -> float:
    return float(statistics.pstdev(xs)) if len(xs) >= 2 else 0.0


def extract_features(p: Problem) -> dict[str, float | int]:
    floor_cells = sum(p.passable)
    walls = p.v - floor_cells
    color_counts = [len(ts) for ts in p.terminals]
    nonzero_counts = [x for x in color_counts if x > 0]
    total_slimes = sum(color_counts)
    small_colors = [c for c, cnt in enumerate(color_counts) if 0 < cnt <= SMALL_COLOR_MAX_SLIMES]
    small_slimes = sum(color_counts[c] for c in small_colors)

    nest_dist = [bfs(p, p.nests[c]) for c in range(p.k)]
    slime_distances: list[int] = []
    per_color_mean_dist: list[float] = []
    for c in range(p.k):
        ds = [nest_dist[c][v] for v in p.terminals[c]]
        ds = [d for d in ds if d < 10**9]
        slime_distances.extend(ds)
        if ds:
            per_color_mean_dist.append(mean_or_zero(ds))

    steiner_edges = [greedy_steiner_edge_count(p, c) for c in range(p.k)]
    finite_steiner = [x for x in steiner_edges if x < 10**9]

    # shortest path demand: そのセルが各terminal->nestの「いずれかの最短路」に含まれる回数。
    color_path_union = [[False] * p.v for _ in range(p.k)]
    demand_values: list[int] = []
    same_overlap_num = 0
    same_overlap_den = 0
    path_peak_demand = 0

    for c in range(p.k):
        demand = [0] * p.v
        dn = nest_dist[c]
        for s in p.terminals[c]:
            ds = bfs(p, s)
            shortest = dn[s]
            if shortest >= 10**9:
                continue
            for v in range(p.v):
                if not p.passable[v]:
                    continue
                if ds[v] + dn[v] == shortest:
                    demand[v] += 1
        for v, d in enumerate(demand):
            if d > 0:
                color_path_union[c][v] = True
                demand_values.append(d)
                same_overlap_num += max(0, d - 1)
                same_overlap_den += d
                path_peak_demand = max(path_peak_demand, d)

    cross_overlap_num = 0
    cross_overlap_den = 0
    cross_peak_colors = 0
    for v in range(p.v):
        if not p.passable[v]:
            continue
        use = sum(1 for c in range(p.k) if color_path_union[c][v])
        if use:
            cross_overlap_num += max(0, use - 1)
            cross_overlap_den += use
            cross_peak_colors = max(cross_peak_colors, use)

    # 通路形状。
    degrees: list[int] = []
    straight_runs: list[int] = []
    for v in range(p.v):
        if not p.passable[v]:
            continue
        x, y = divmod(v, p.n)
        degrees.append(sum(1 for _ in neighbors(p, v)))
        best = 0
        for dx, dy in zip(DX, DY):
            curx, cury = x, y
            length = 0
            for _ in range(8):
                curx += dx
                cury += dy
                if not (0 <= curx < p.n and 0 <= cury < p.n):
                    break
                if p.grid[curx][cury] == "#":
                    break
                length += 1
            best = max(best, length)
        straight_runs.append(best)

    nest_degrees = [sum(1 for _ in neighbors(p, p.nests[c])) for c in range(p.k)]

    mean_count = mean_or_zero(color_counts)
    return {
        "N": p.n,
        "K": p.k,
        "floor_cells": floor_cells,
        "wall_cells": walls,
        "wall_ratio": walls / p.v if p.v else 0.0,
        "total_slimes": total_slimes,
        "color_count_mean": mean_count,
        "color_count_std": pstdev_or_zero(color_counts),
        "color_count_cv": pstdev_or_zero(color_counts) / mean_count if mean_count > 0 else 0.0,
        "color_count_min_nonzero": min(nonzero_counts) if nonzero_counts else 0,
        "color_count_max": max(color_counts) if color_counts else 0,
        "small_color_count": len(small_colors),
        "small_color_ratio": len(small_colors) / p.k if p.k else 0.0,
        "small_slime_count": small_slimes,
        "small_slime_ratio": small_slimes / total_slimes if total_slimes else 0.0,
        "distance_sum": sum(slime_distances),
        "distance_mean": mean_or_zero(slime_distances),
        "distance_std": pstdev_or_zero(slime_distances),
        "distance_max": max(slime_distances) if slime_distances else 0,
        "color_mean_distance_std": pstdev_or_zero(per_color_mean_dist),
        "steiner_edges_sum": sum(finite_steiner),
        "steiner_edges_mean": mean_or_zero(finite_steiner),
        "steiner_edges_max": max(finite_steiner) if finite_steiner else 0,
        "steiner_edges_per_slime": sum(finite_steiner) / total_slimes if total_slimes else 0.0,
        "same_color_shortest_path_overlap": same_overlap_num / same_overlap_den if same_overlap_den else 0.0,
        "path_peak_demand": path_peak_demand,
        "path_demand_mean": mean_or_zero(demand_values),
        "cross_color_shortest_path_overlap": cross_overlap_num / cross_overlap_den if cross_overlap_den else 0.0,
        "cross_path_peak_colors": cross_peak_colors,
        "floor_degree1_ratio": sum(d == 1 for d in degrees) / len(degrees) if degrees else 0.0,
        "floor_degree2_ratio": sum(d == 2 for d in degrees) / len(degrees) if degrees else 0.0,
        "floor_degree3plus_ratio": sum(d >= 3 for d in degrees) / len(degrees) if degrees else 0.0,
        "straight_run_mean": mean_or_zero(straight_runs),
        "straight_run_max": max(straight_runs) if straight_runs else 0,
        "straight_run_ge4_ratio": sum(x >= 4 for x in straight_runs) / len(straight_runs) if straight_runs else 0.0,
        "straight_run_ge7_ratio": sum(x >= 7 for x in straight_runs) / len(straight_runs) if straight_runs else 0.0,
        "nest_degree_mean": mean_or_zero(nest_degrees),
        "nest_degree_min": min(nest_degrees) if nest_degrees else 0,
    }


def safe_case_name(rel: str) -> str:
    return re.sub(r"[^A-Za-z0-9_.-]+", "_", rel)


def compile_cpp(source: Path, output: Path, cxx: str, flags: list[str]) -> None:
    output.parent.mkdir(parents=True, exist_ok=True)
    cmd = [cxx, *flags, str(source), "-o", str(output)]
    print("compile:", " ".join(shlex.quote(x) for x in cmd), file=sys.stderr)
    subprocess.run(cmd, check=True)


@dataclass
class RunResult:
    actions: int | None
    runtime_ms: float
    returncode: int | None
    timed_out: bool
    stderr: str


def run_solver(exe: Path, input_bytes: bytes, timeout_sec: float) -> RunResult:
    begin = time.perf_counter()
    try:
        cp = subprocess.run(
            [str(exe)],
            input=input_bytes,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            timeout=timeout_sec,
            check=False,
        )
        elapsed = (time.perf_counter() - begin) * 1000.0
        stdout = cp.stdout.decode("utf-8", errors="replace")
        stderr = cp.stderr.decode("utf-8", errors="replace")
        actions = sum(1 for line in stdout.splitlines() if line.strip())
        return RunResult(actions, elapsed, cp.returncode, False, stderr)
    except subprocess.TimeoutExpired as e:
        elapsed = (time.perf_counter() - begin) * 1000.0
        stderr_b = e.stderr or b""
        if isinstance(stderr_b, str):
            stderr = stderr_b
        else:
            stderr = stderr_b.decode("utf-8", errors="replace")
        return RunResult(None, elapsed, None, True, stderr)


def parse_key_values(line: str) -> dict[str, str]:
    result: dict[str, str] = {}
    for key, value in re.findall(r"([A-Za-z_][A-Za-z0-9_]*)=([^\s,]+)", line):
        result[key] = value
    return result


def to_number(value: str | None):
    if value is None:
        return ""
    v = value.strip()
    if v == "none":
        return ""
    try:
        if any(c in v for c in ".eE"):
            return float(v)
        return int(v)
    except ValueError:
        return v


def parse_pooled_log(stderr: str) -> dict[str, object]:
    out: dict[str, object] = {}
    for line in stderr.splitlines():
        if line.startswith("initial lookahead_baseline="):
            kv = parse_key_values(line)
            for k in ("lookahead_baseline", "build_ms", "first_keep", "setup_keep"):
                if k in kv:
                    out[f"pooled_{k}"] = to_number(kv[k])
            m = re.search(r"alpha=([^\s]+)", line)
            if m:
                out["pooled_alpha"] = m.group(1)
        elif line.startswith("sa final="):
            kv = parse_key_values(line)
            wanted = (
                "final", "current", "elapsed_ms", "trials", "direct_trials", "beam_trials", "valid", "accepted",
                "improve", "neutral", "uphill", "beam_deep", "refresh", "generated_total", "direct_pool_total",
                "beam_pool_total", "prefilter_direct", "prefilter_reject", "direct_valid", "direct_use_fail",
                "direct_rejoin_fail", "direct_steps", "reroutes", "beam_valid", "beam_states", "raw", "children",
                "rejoins",
            )
            for k in wanted:
                if k in kv:
                    out[f"pooled_{k}"] = to_number(kv[k])
    return out


def parse_structured_log(stderr: str) -> dict[str, object]:
    out: dict[str, object] = {}
    for line in stderr.splitlines():
        if line.startswith("small mixed prefix:"):
            m = re.search(r"small mixed prefix:\s*(?:(\d+) actions|failed),\s*normal=(\d+),\s*recovery=(\d+),\s*recovery_calls=(\d+)", line)
            if m:
                out["structured_mixed_prefix_actions"] = int(m.group(1)) if m.group(1) else ""
                out["structured_mixed_prefix_success"] = 1 if m.group(1) else 0
                out["structured_mixed_prefix_normal"] = int(m.group(2))
                out["structured_mixed_prefix_recovery"] = int(m.group(3))
                out["structured_mixed_prefix_recovery_calls"] = int(m.group(4))
        elif line.startswith("fill8/flush"):
            m = re.match(
                r"fill8/flush(\d+): best=([^,]+), current=([^,]+), mixed=([^,]+), trials=(\d+), mixed_wins=(\d+)",
                line,
            )
            if m:
                x = m.group(1)
                out[f"structured_flush{x}_best"] = to_number(m.group(2))
                current = m.group(3)
                cm = re.match(r"(\d+)\(insert=(\d+)\)", current)
                if cm:
                    out[f"structured_flush{x}_current"] = int(cm.group(1))
                    out[f"structured_flush{x}_insert"] = int(cm.group(2))
                else:
                    out[f"structured_flush{x}_current"] = to_number(current)
                    out[f"structured_flush{x}_insert"] = ""
                out[f"structured_flush{x}_mixed"] = to_number(m.group(4))
                out[f"structured_flush{x}_trials"] = int(m.group(5))
                out[f"structured_flush{x}_mixed_wins"] = int(m.group(6))
        elif line.startswith("total trials="):
            try:
                out["structured_total_trials"] = int(line.split("=", 1)[1])
            except ValueError:
                pass
    return out


def discover_inputs(input_dir: Path, pattern: str, recursive: bool) -> list[Path]:
    candidates = input_dir.rglob(pattern) if recursive else input_dir.glob(pattern)
    files = [p for p in candidates if p.is_file()]
    return sorted(files, key=lambda p: str(p.relative_to(input_dir)))


FEATURE_COLUMNS = [
    "N", "K", "floor_cells", "wall_cells", "wall_ratio", "total_slimes",
    "color_count_mean", "color_count_std", "color_count_cv", "color_count_min_nonzero", "color_count_max",
    "small_color_count", "small_color_ratio", "small_slime_count", "small_slime_ratio",
    "distance_sum", "distance_mean", "distance_std", "distance_max", "color_mean_distance_std",
    "steiner_edges_sum", "steiner_edges_mean", "steiner_edges_max", "steiner_edges_per_slime",
    "same_color_shortest_path_overlap", "path_peak_demand", "path_demand_mean",
    "cross_color_shortest_path_overlap", "cross_path_peak_colors",
    "floor_degree1_ratio", "floor_degree2_ratio", "floor_degree3plus_ratio",
    "straight_run_mean", "straight_run_max", "straight_run_ge4_ratio", "straight_run_ge7_ratio",
    "nest_degree_mean", "nest_degree_min",
]


def write_csv(path: Path, rows: list[dict[str, object]]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    if not rows:
        path.write_text("", encoding="utf-8")
        return
    keys: list[str] = []
    seen: set[str] = set()
    preferred = [
        "case", *FEATURE_COLUMNS,
        "pooled_actions", "structured_actions", "action_diff_pooled_minus_structured", "relative_diff", "winner",
        "pooled_runtime_ms", "structured_runtime_ms", "pooled_returncode", "structured_returncode",
        "pooled_timed_out", "structured_timed_out",
    ]
    for k in preferred:
        if any(k in row for row in rows) and k not in seen:
            keys.append(k); seen.add(k)
    for row in rows:
        for k in row:
            if k not in seen:
                keys.append(k); seen.add(k)

    with path.open("w", newline="", encoding="utf-8") as f:
        writer = csv.DictWriter(f, fieldnames=keys, extrasaction="ignore")
        writer.writeheader()
        writer.writerows(rows)


def feature_summary(rows: list[dict[str, object]]) -> list[dict[str, object]]:
    pooled_rows = [r for r in rows if r.get("winner") == "pooled"]
    structured_rows = [r for r in rows if r.get("winner") == "structured"]
    result: list[dict[str, object]] = []

    for feature in FEATURE_COLUMNS:
        a = [float(r[feature]) for r in pooled_rows if isinstance(r.get(feature), (int, float))]
        b = [float(r[feature]) for r in structured_rows if isinstance(r.get(feature), (int, float))]
        if not a or not b:
            continue
        ma, mb = mean_or_zero(a), mean_or_zero(b)
        combined = a + b
        sd = pstdev_or_zero(combined)
        effect = (ma - mb) / sd if sd > 1e-12 else 0.0
        result.append({
            "feature": feature,
            "pooled_win_mean": ma,
            "structured_win_mean": mb,
            "difference": ma - mb,
            "standardized_difference": effect,
            "pooled_win_cases": len(a),
            "structured_win_cases": len(b),
        })

    result.sort(key=lambda r: abs(float(r["standardized_difference"])), reverse=True)
    return result


def main() -> int:
    ap = argparse.ArgumentParser(
        description="AHC072: pooled履歴修正型とfill8/mixed構築型を多数ケースで比較し、入力特徴量をCSV化する。"
    )
    ap.add_argument("--inputs", type=Path, required=True, help="入力ケースを置いたディレクトリ")
    ap.add_argument("--pattern", default="*", help="入力ファイルglob (default: *)")
    ap.add_argument("--recursive", action="store_true", help="入力ディレクトリを再帰探索")
    ap.add_argument("--pooled-source", type=Path, default=Path("ahc072_sa_pooled_lookahead_a025.cpp"))
    ap.add_argument("--structured-source", type=Path, default=Path("ahc072_fill8_mixed_compare.cpp"))
    ap.add_argument("--build-dir", type=Path, default=Path(".compare_build"))
    ap.add_argument("--output", type=Path, default=Path("case_comparison.csv"))
    ap.add_argument("--summary-output", type=Path, default=Path("case_feature_summary.csv"))
    ap.add_argument("--log-dir", type=Path, default=Path("case_compare_logs"))
    ap.add_argument("--cxx", default="g++")
    ap.add_argument("--cxxflags", default="-std=c++23 -O2 -pipe")
    ap.add_argument("--timeout", type=float, default=5.0, help="各solverの外部timeout秒")
    ap.add_argument("--limit", type=int, default=0, help="先頭Nケースだけ試す。0は全件")
    ap.add_argument("--skip-compile", action="store_true", help="build-dir内の既存実行ファイルを使う")
    args = ap.parse_args()

    if not args.inputs.is_dir():
        ap.error(f"input directory not found: {args.inputs}")

    cases = discover_inputs(args.inputs, args.pattern, args.recursive)
    if args.limit > 0:
        cases = cases[: args.limit]
    if not cases:
        ap.error("no input files found")

    pooled_exe = args.build_dir / "pooled"
    structured_exe = args.build_dir / "structured"
    if not args.skip_compile:
        flags = shlex.split(args.cxxflags)
        compile_cpp(args.pooled_source, pooled_exe, args.cxx, flags)
        compile_cpp(args.structured_source, structured_exe, args.cxx, flags)
    elif not pooled_exe.exists() or not structured_exe.exists():
        ap.error("--skip-compile specified but executables are missing")

    pooled_exe = pooled_exe.resolve()
    structured_exe = structured_exe.resolve()
    args.log_dir.mkdir(parents=True, exist_ok=True)

    rows: list[dict[str, object]] = []
    total = len(cases)

    for idx, path in enumerate(cases, 1):
        rel = str(path.relative_to(args.inputs))
        print(f"[{idx}/{total}] {rel}", file=sys.stderr, flush=True)
        try:
            problem = parse_problem(path)
            features = extract_features(problem)
        except Exception as e:
            print(f"  feature extraction failed: {e}", file=sys.stderr)
            continue

        data = path.read_bytes()
        pooled = run_solver(pooled_exe, data, args.timeout)
        structured = run_solver(structured_exe, data, args.timeout)

        row: dict[str, object] = {"case": rel, **features}
        row.update({
            "pooled_actions": pooled.actions if pooled.actions is not None else "",
            "structured_actions": structured.actions if structured.actions is not None else "",
            "pooled_runtime_ms": round(pooled.runtime_ms, 3),
            "structured_runtime_ms": round(structured.runtime_ms, 3),
            "pooled_returncode": pooled.returncode if pooled.returncode is not None else "",
            "structured_returncode": structured.returncode if structured.returncode is not None else "",
            "pooled_timed_out": int(pooled.timed_out),
            "structured_timed_out": int(structured.timed_out),
        })
        row.update(parse_pooled_log(pooled.stderr))
        row.update(parse_structured_log(structured.stderr))

        if pooled.actions is not None and structured.actions is not None:
            diff = pooled.actions - structured.actions
            row["action_diff_pooled_minus_structured"] = diff
            row["relative_diff"] = diff / structured.actions if structured.actions else 0.0
            row["winner"] = "pooled" if diff < 0 else "structured" if diff > 0 else "tie"
        else:
            row["action_diff_pooled_minus_structured"] = ""
            row["relative_diff"] = ""
            row["winner"] = "error"

        safe = safe_case_name(rel)
        (args.log_dir / f"{safe}.pooled.stderr.txt").write_text(pooled.stderr, encoding="utf-8")
        (args.log_dir / f"{safe}.structured.stderr.txt").write_text(structured.stderr, encoding="utf-8")
        rows.append(row)

        # 途中終了しても結果が残るよう、毎ケースCSVを書き直す。
        write_csv(args.output, rows)

        pa = pooled.actions if pooled.actions is not None else "ERR"
        sa = structured.actions if structured.actions is not None else "ERR"
        print(f"  pooled={pa} structured={sa} winner={row['winner']}", file=sys.stderr, flush=True)

    summary = feature_summary(rows)
    write_csv(args.summary_output, summary)

    valid = [r for r in rows if r.get("winner") in ("pooled", "structured", "tie")]
    pw = sum(r.get("winner") == "pooled" for r in valid)
    sw = sum(r.get("winner") == "structured" for r in valid)
    tw = sum(r.get("winner") == "tie" for r in valid)
    diffs = [float(r["action_diff_pooled_minus_structured"]) for r in valid if isinstance(r.get("action_diff_pooled_minus_structured"), (int, float))]

    print("\n=== summary ===", file=sys.stderr)
    print(f"valid={len(valid)} pooled_wins={pw} structured_wins={sw} ties={tw}", file=sys.stderr)
    if diffs:
        print(f"mean(pooled-structured)={mean_or_zero(diffs):.3f}", file=sys.stderr)
    if summary:
        print("top distinguishing input features (correlation, not causation):", file=sys.stderr)
        for r in summary[:10]:
            effect = float(r["standardized_difference"])
            direction = "higher in pooled-win" if effect > 0 else "higher in structured-win"
            print(f"  {r['feature']}: z-diff={effect:+.3f} ({direction})", file=sys.stderr)

    print(f"CSV: {args.output}", file=sys.stderr)
    print(f"feature summary: {args.summary_output}", file=sys.stderr)
    print(f"logs: {args.log_dir}", file=sys.stderr)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
