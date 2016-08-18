# Tag-and-Probe Utilities

This repository holds a set of utilities/macros to work with Tag-and-Probe (TnP) ntuples.

All script are written in python and take a `--help` argument, which describes the functionality of the different scripts. The following sections explain the usage in simple use-cases. All of them should work with a setup CMS software (CMSSW) environment.

## Compare TnP trees

The script `compareTrees` can be used to check that the output ROOT files of different CMSSW package versions is still the same. Run following example to examine the output of the script.

```bash
# Take the example tree, remove the eta branch and set all values of the pt branch to zero
./modifyTree exampleTree.root modifiedTree.root --remove eta --branch pt

# Now, examine the differences using the 'compareTrees' tool
./compareTrees modifiedTree.root exampleTree.root
```
## Skim TnP tree

With the program `subTree`, you can reduce TnP ROOT files by applying cuts on a specified input tree and copy the result to an output ROOT file. This is mainly done to reduce the file size and therefore to reduce the needed processing time for TnP studies.

```bash
./skimTree exampleTree.root skimmedTree.root --cut "tag_IsoMu20==1 && tag_pt>30"
```

## Writing outputs to file

All programs do not have a specific option to write the output to a file because this can be done directly in the terminal.

```bash
./someProgram > outputFile
```
