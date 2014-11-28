104P is a set of Common Image & Video Processing tools.

Number 104 in the name can be writtent as CIV in the roman numbers. Being considered as an acronym, CIV gives "Common Image & Video" and then letter P stands for "Processing". Moreover, there were exactely 104 commits when I desided to split this repo out of my main code base.

Summary:
104Core - basic data structures (like Image, Point, etc.)
104Algo - some very common algorithms (like convolution, sampling, etc.)
104IO - image and flow IO, plus some helpers for visualization
104Param - classes and interfaces used for automatic UI building for algorithm parameterization. Separated due to the dependency on sigc++ lib.
104UI - framework for building UI using gtkmm. 
[Planned] 104Example - example of the usage of the libs listed above.

Dependencies:
104Core - NO
104Algo - 104Core
104IO - 104Core + IIO and TCLAP which are embedded
104Param - sigc++
104UI - 104Core, 104Algo, 104IO, 104Param, gtkmm
