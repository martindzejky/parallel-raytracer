# Configurations

Here are various OpenMP+MPI configurations and the results that I've got when running this program.

| OpenMP threads | MPI machines | Time to render a frame |
|:--------------:|:------------:|:----------------------:|
| 1              | 1            | 8.5-10 seconds         |
| 2              | 1            | 5-6 seconds            |
| 8              | 1            | 2.5-3 seconds          |
| 8+?            | 2            | ??? seconds            |

## Constructs

I only use a limited subset of provided OpenMP and MPI commands and constructs. This is because the program itself is simple. For every pixel, run a function that produces color. It's just one huge `for` loop.

From OpenMP, there's just `omp parallel for`. This creates worker threads that execute the function for every pixel in the main loop. There's no point in parallelizing anything in the tracing algorithm, because it's just executed by one worker. There's never more workers that `for` loop iterations.

From MPI, I only use `MPI_Send`, `MPI_Rect`, and `MPI_Iprobe` (along with the initialization, finalization, and functions for polling the size and rank). The reason is simple. Each machine gets it's range in the main `for` loop in the start. The following communication looks like this: every slave get's a timestamp and produces colors for this timestamp (for its respective `for` loop section). Master runs its own part, gathers remaining pixel colors from slaves, and finally renders the image.
