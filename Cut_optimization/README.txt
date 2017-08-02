# Cut_optimization
Cut optimization to maximize signal significance.

===================================================================

InputChain: Supporting class for TMVA and TryCuts (see below). Specifies tree files with regularly indexed names to use to generate histograms.
    - See InputChain directory for instructions on use.

ROOT_Util: Contains generally useful functions for the project. (Static library + template functions)

TMVA: Contains tools for finding cuts to optimize signal significance
    - Divided into three programs:
        1. myTMVA
        2. readxml
        3. readxml_fit
    - See the TMVA directory for instructions on use

TryCuts: Contains a program for trying all combinations of cuts within a discrete, user-defined parameter space to see the effects on signal significance.
    - Divided into two programs:
        1. TryCuts
        2. TryCuts_fit
    - See the TryCuts directory for instructions on use.