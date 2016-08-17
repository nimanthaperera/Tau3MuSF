# Tag-and-Probe Utilities

This repository holds a set of utilities/macros to work with Tag-and-Probe (TnP) ntuples.

All script are written in python and take a `--help` argument, which describes the functionality of the different scripts. The following sections explain the usage in simple use-cases. All of them should work with a setup CMS software (CMSSW) environment.

## Compare TnP trees

The script `compareTrees` can be used to check that the output ROOT files of different CMSSW package versions is still the same. Run following example to examine the output of the script.

```bash
# Take the example tree, remove the eta branch and set all values of the pt branch to zero
./modifyTree exampleTree.root exampleTreeModified.root --remove eta --branch pt

# Now, examine the differences using the 'compareTrees' tool
./compareTrees exampleTreeModified.root exampleTree.root
```

