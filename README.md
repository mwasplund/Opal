# Opal
Open Platform Abstraction Layer

## Benchmarks
My old Dell XPS 15 with Win11

|               ns/op |                op/s |    err% |     total | benchmark
|--------------------:|--------------------:|--------:|----------:|:----------
|               99.39 |       10,061,704.21 |    4.3% |      0.01 | `Concatenate Paths`
|            1,360.93 |          734,790.81 |    1.9% |      0.17 | `Concatenate Paths With Up Reference`
|               12.25 |       81,630,676.41 |    2.3% |      0.01 | `Initialize Path With Root`
|               22.70 |       44,054,327.18 |    1.9% |      0.03 | `Create Windows Path With Root`
|               12.25 |       81,627,610.39 |    2.3% |      0.01 | `Initialize Path Relative`
|            2,483.51 |          402,656.33 |    3.7% |      0.30 | `Get User Profile Directory`
|              652.29 |        1,533,060.45 |    4.5% |      0.08 | `Get Current Directory`
|               10.78 |       92,772,984.51 |    2.5% |      0.01 | `SemanticVersion ToString Major Only`
|               24.67 |       40,529,523.51 |    2.6% |      0.03 | `SemanticVersion ToString With Minor`
|               38.48 |       25,984,810.93 |    3.5% |      0.05 | `SemanticVersion ToString With Minor and Path`
|               14.03 |       71,251,652.40 |    2.3% |      0.02 | `SemanticVersion Parse Major Only`
|               26.83 |       37,268,496.07 |    2.2% |      0.03 | `SemanticVersion Parse With Minor`
|               38.17 |       26,197,729.46 |    3.9% |      0.05 | `SemanticVersion Parse With Minor and Path`