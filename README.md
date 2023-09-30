# 3Way_Matching

The purpose of this repo is to provide a library for three-way matching of propensity scores.

## Getting Started
This function provides a Makefile, so if you have a C++ compiler installed (the Makefile uses g++, but you can change it
as you require), you should be able to create the executable by navigating to this directory in a shell and calling `make`.

## Using This Library

The expected usage of this library is to create three-way matches of propensity scores.

Put the patient records in a CSV file with the following structure:
`ID / GROUP / PSCORE1 / PSCORE2` where:
- ID is a unique integer
- GROUP is 1, 2, or 3, where 1=RED, 2=BLUE, 3=GREEN
- PSCORE1 and PSCORE2 are the propensity scores
- Note that the column names don't actually matter, but the order and data type does
We will call this file `./data/example_data.csv` for the sake of illustration.

## Expected Inputs

We call the function like:

`./3Way_Matching ./data/example_data.csv ./output.csv -v`

The `-v` verbose option is included if you want step-by-step printouts to indicate progress and matches as they happen. 

If the input file ends in .csv, then commas will be used as the delimiter. Otherwise, whitespace will be used.

There is also a file threshold.in, which the program expects to be present in the same directory. If it isn't
present or doesn't contain a valid value, the program defaults to using 0.25 as the caliper threshold for
maximum match distance (that is, the sum of the distances between all points in the match). If you would
like to use a different value, simply change the value in threshold.in

## Expected Outputs
The expected output is a list of matches in order from smallest distance to largest. For example:

`1,2,3`

`4,5,6`

If patients [1, 2, 3] were matched together with less distance than the other match which has patients [4, 5, 6]

The output is printed to the specified output file.
