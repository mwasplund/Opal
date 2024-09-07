# Opal
Open Platform Abstraction Layer

## Benchmarks
My old Dell XPS 15 with Win11

|               ns/op |                op/s |    err% |     total | benchmark
|--------------------:|--------------------:|--------:|----------:|:----------
|              472.95 |        2,114,401.83 |    1.9% |      0.06 | `Concatenate Paths`
|            4,188.87 |          238,727.87 |    1.9% |      0.50 | `Concatenate Paths With Up Reference`
|              172.74 |        5,789,114.15 |    1.4% |      0.21 | `Initialize Path With Root`
|              274.30 |        3,645,670.04 |    1.7% |      0.33 | `Create Windows Path With Root`
|              190.91 |        5,238,194.79 |    2.8% |      0.23 | `Initialize Path Relative`
|            2,404.53 |          415,882.44 |    3.7% |      0.30 | `Get User Profile Directory`
|            2,756.76 |          362,744.38 |    1.6% |      0.33 | `Get Current Directory`
|              123.78 |        8,078,929.17 |    1.6% |      0.15 | `SemanticVersion ToString Major Only`
|              263.52 |        3,794,825.21 |    3.0% |      0.32 | `SemanticVersion ToString With Minor`
|              385.18 |        2,596,170.08 |    0.9% |      0.48 | `SemanticVersion ToString With Minor and Path`