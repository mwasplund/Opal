# Opal
Open Platform Abstraction Layer

## Benchmarks

|               ns/op |                op/s |    err% |     total | benchmark
|--------------------:|--------------------:|--------:|----------:|:----------
|              472.95 |        2,114,401.83 |    1.9% |      0.06 | `Concatenate Paths`
|            4,188.87 |          238,727.87 |    1.9% |      0.50 | `Concatenate Paths With Up Reference`
|              172.74 |        5,789,114.15 |    1.4% |      0.21 | `Initialize Path With Root`
|              190.91 |        5,238,194.79 |    2.8% |      0.23 | `Initialize Path Relative`
|            2,404.53 |          415,882.44 |    3.7% |      0.30 | `Get User Profile Directory`
|            2,756.76 |          362,744.38 |    1.6% |      0.33 | `Get Current Directory`