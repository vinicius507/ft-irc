#!/usr/bin/env bash

FILES=$@

if ! clang-format --Werror --dry-run $FILES; then
  clang-format -i --Werror $FILES
  exit 1
fi
