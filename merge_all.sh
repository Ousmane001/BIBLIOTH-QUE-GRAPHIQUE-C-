#!/bin/bash


git checkout $2
git add .
git commit -m "preparation au merge"
git merge $1



git checkout $3
git add .
git commit -m "preparation au merge"
git merge $1

git checkout $4
git add .
git commit -m "preparation au merge"
git merge $1

