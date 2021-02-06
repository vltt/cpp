#!/bin/bash

# better use as alias in ~/.bashrc

# apply clang-format for staged and unstaged files
git diff HEAD --name-only | grep -E ".+\.(h|c|hpp|cpp)" | xargs -t clang-format -i