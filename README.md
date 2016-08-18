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

With the program `subTree`, you can reduce TnP ROOT files by applying cuts on a specified input tree and copy the result to an output ROOT file. As well, you can remove branches from the tree completely. This is mainly done to reduce the file size and therefore to reduce the needed processing time for TnP studies.

```bash
# Skim a tree only with a cut string
./skimTree exampleTree.root skimmedTree.root --cut "tag_IsoMu20==1 && tag_pt>30"

# Remove some specific branches from a tree
./skimTree exampleTree.root skimmedTree.root --remove "tag_* pt eta abseta"

# Remove all branches from the tree except the specified ones
# Here, first you remove all of the branches and re-add only the desired ones
./skimTree exampleTree.root skimmedTree.root --remove "*" --keep "pt eta"
```

## Print TnP tree

You can print run, luminosity and event of a TnP tree with a specified cut. This is used mainly to feed the output to [this CMS tool](https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookPickEvents). Most likely, you want to pipe the output to a file.

```bash
# Print output to the terminal (stdout)
./printTree exampleTree.root --cut "tag_IsoMu20==1 && tag_pt>30"

# Write output to file
./printTree exampleTree.root --cut "tag_IsoMu20==1 && tag_pt>30" > outputFile
```

## Create cut string from JSON file

The script `jsonToCut` generates a valid cut string from a JSON file, which specifies runs and luminosity sections (see `exampleCut.json`). Following example shows how you can feed the cut string into the other scripts without doing copy past in the terminal.

```bash
# Read the example JSON file to a shell variable
CUT=$(./jsonToCut exampleCut.json)

# Have a look at the cut string
echo $CUT

# Then, feed the cut to any other program, which takes a cut string
# Note, that you have to put "" around the variable
./printTree exampleTree.root --cut "$CUT"
```

As well, you can combine multiple cuts using this approach. Here a combination of cuts created from a JSON file and a cut from a string.

```bash
# Declare cut from JSON file to as shell variable
CUT1=$(./jsonToCut exampleCut.json)

# Get cut from string
CUT2="tag_IsoMu20==1 && tag_pt>30"

# Combine the cuts with a && operation
# Note, that you have to use () around the variables to get the desired logic!
./printTree exampleTree.root --cut "($CUT1) && ($CUT2)"
```

## Writing outputs to file

Remember, all programs do not have a specific option to write the output to a file because this can be done directly in the terminal.

```bash
./someProgram > outputFile
```
