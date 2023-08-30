#!/usr/bin/bash
set -e

testfile="$1"
database="$2"

echo "Creating database ..."
./"$1" "$2".dat c
echo ""

echo "Saving zed, frank, joe to database ..."
./"$1" "$2".dat s 1 zed zed@zedshaw.com
./"$1" "$2".dat s 2 frank frank@zedshaw.com
./"$1" "$2".dat s 3 joe joe@zedshaw.com
echo ""

echo "Printing all records ..."
output=$(./"$1" "$2".dat l)
./"$1" "$2".dat l
echo $output
echo ""

echo "Deleting record by id 3 ..."
./"$1" "$2".dat d 3
echo ""

echo "Printing all records ..."
./"$1" "$2".dat l
echo ""

echo "Getting record by id 2 ..."
./"$1" "$2".dat g 2
