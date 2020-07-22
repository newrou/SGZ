#!/bin/bash

Receptor=`basename $1 .pdbqt`
Ligand=`basename $2 .pdbqt`
Box=`./get-mol-box.py < $1`

echo $Box

vina --receptor $Receptor.pdbqt --ligand $Ligand.pdbqt $Box --log $Receptor~$Ligand.log --out $Receptor~$Ligand.pdbqt | tee $Receptor~$Ligand.dlg

#  --receptor arg             rigid part of the receptor (PDBQT)
#  --flex arg                 flexible side chains, if any (PDBQT)
#  --ligand arg               ligand (PDBQT)
#Search space (required):
#  --center_x arg             X coordinate of the center
#  --center_y arg             Y coordinate of the center
#  --center_z arg             Z coordinate of the center
#  --size_x arg               size in the X dimension (Angstroms)
#  --size_y arg               size in the Y dimension (Angstroms)
#  --size_z arg               size in the Z dimension (Angstroms)
#Output (optional):
#  --out arg                  output models (PDBQT), the default is chosen based
#                             on the ligand file name
#  --log arg                  optionally, write log file
#Misc (optional):
#  --cpu arg                  the number of CPUs to use (the default is to try 
#                             to detect the number of CPUs or, failing that, use
#                             1)
#  --seed arg                 explicit random seed
#  --exhaustiveness arg (=8)  exhaustiveness of the global search (roughly 
#                             proportional to time): 1+
#  --num_modes arg (=9)       maximum number of binding modes to generate
#  --energy_range arg (=3)    maximum energy difference between the best binding
#                             mode and the worst one displayed (kcal/mol)
#Configuration file (optional):
#  --config arg               the above options can be put here
#Information (optional):
#  --help                     display usage summary
#  --help_advanced            display usage summary with advanced options
#  --version                  display program version
