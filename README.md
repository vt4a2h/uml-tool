[![Build Status](https://travis-ci.org/vt4a2h/uml-tool.svg?branch=master)](https://travis-ci.org/vt4a2h/uml-tool/builds)
<a href="https://scan.coverity.com/projects/5161">
  <img alt="Coverity Scan Build Status"
       src="https://scan.coverity.com/projects/5161/badge.svg"/>
</a>
[![Coverage Status](https://coveralls.io/repos/vt4a2h/uml-tool/badge.svg?branch=master)](https://coveralls.io/r/vt4a2h/uml-tool?branch=master)

# UML tool for Qt

## General information:
This tool is for creating UML class diagrams with taking into account Qt Framework specific (slots/signals/properties etc.). It is possible to create a class diagram and generate the project based on it with this tool. The tool can be used for fast prototyping and generating boilerplate code.

Please note that this project is under development. There are still no stable releases so far. 
This is independent project and is not affiliated with The Qt Company and its beneficiaries.

## Dependencies:
- Qt    >= 5.12
- gcc   >= 8
- gtest (the latest one). Only for tests.
- CMake >= 3.13.0 (If you prefer it, but you can use qmake for building). Supplied as a part of submodule uml-tool-dependencies.

## Submodules:
- boost-di
- ranges-v3
- uml-tool-dependencies

## C++ standard: 
C++17 (w/o extensions)

## License: 
LGPL v3.

## Notes:
*Tested and built only on Linux. Patches are welcome, feel free to add other systems support or implement some new functionality.*
