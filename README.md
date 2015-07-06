# Analysis and Plotting in ROOT
This is an analysis package using the ROOT framework from CERN for parsing input files and generating plots with a variety of data inputs.

## ROOT
Root is a data analysis framework created at CERN to provide analysis and plotting tools for High Energy Physics.  It is used primarily in the High Energy Experimental community, but also sees use among Particle Theorists.

ROOT is written in C++, and allows for input through a command line interface, through scripts, and through direct inclusion in other C++ code.

http://root.cern.ch/drupal/

## This Package
This package uses ROOT as a back-end for plotting, but streamlines the analysis portion for faster adaptability, and provides a more streamlined input interface.  The inputs in ROOT analysis scripts are often unintuitive, and various related flags can be set at different unintuitive locations.  This package also allows a reasonably broad set of formulas to be input, as well as applying experimental constraints on parameter space.

Separate from the plotting tools, this package also includes tools for parsing inputs from Les Houches Accord (LHA) and LHC Olympics (LHCO) files, used to store particle spectra and simulated collider events respectively.

## Requirements
This package requires ROOT to be installed on the system.  A template input file is included, and can be run from the main program directory.  Aggregate routines expect the presence of a series of .lha/.slha or .lhco files with regular naming schemes, for example

    xxxx[0-9]yyyy[0-9]zzzz[0-9].lha,

while plotting routines expect tab- or space- separated columns of input numerals with the option of a header line.
