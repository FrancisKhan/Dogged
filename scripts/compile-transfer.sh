#!/bin/bash

build_linux () {
  echo "Build Almost for Linux"
  cd ..
  
  if [ $1 == "clean" ]; then
    git clean -x -f -d
  else
    echo "git clean not used" 
  fi
  
  cmake -DCMAKE_BUILD_TYPE=Release .
  make
  cpack
  cp -fv ./app/almost ../ALMOST/app
  cp -fv almost*.deb ../ALMOST/installers
  cp -fv ./tests/test_almost ../ALMOST/tests
}

build_windows () {
  echo "Build Almost for Windows"
  cd ..
  
  if [ $1 == "clean" ]; then
    git clean -x -f -d
  else
    echo "git clean not used" 
  fi

  cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=mingw-w64_toolchain.cmake .
  make
  cd app
  makensis -V1 almost_installer.nsi 
  # get last tag
  cd ..
  FullTag=`git describe --abbrev=0`
  Tag=${FullTag:1}
  cd app
  cp -fv almost.exe ../../ALMOST/app
  cp -fv Almost_installer.exe ../../ALMOST/installers/Almost-${Tag}_installer.exe
  cp -fv ../tests/test_almost.exe ../../ALMOST/tests
}

if [ $# -eq 0 ]
  then
    echo "No arguments supplied, please OS and git clean/keep parameters"
fi

if [ $1 == "linux" ]; then

  if [ $2 == "clean" ]; then
    build_linux clean
  elif [ $2 == "keep" ]; then
    build_linux keep
  else
    echo "git parameter not recognized, use: clean or keep" 
  fi
  
elif [ $1 == "windows" ]; then

  if [ $2 == "clean" ]; then
    build_windows clean
  elif [ $2 == "keep" ]; then
    build_windows keep
  else
    echo "git parameter not recognized, use: clean or keep" 
  fi

elif [ $1 == "all" ]; then
  
  if [ $2 == "clean" ]; then
    build_linux clean
    build_windows clean
  elif [ $2 == "keep" ]; then
    build_linux keep
    build_windows keep
  else
    echo "Make parameter not recognized, use: clean or keep" 
  fi
  
else
  echo "OS system not recognized or not supported." 
  echo "Please choose between: linux, windows or all"
fi

