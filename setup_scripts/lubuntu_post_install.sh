#!/bin/bash
# 
# 	Post install script to be run after Lubuntu install. 
# 	Written with scientific computing applications in mind.
#
#	Author: 	Murray Cutforth
#	Date:		01/09/2017

sudo apt-get update -y
sudo apt-get upgrade -y


# Extra compilers to install (we already have Python)
NEWCOMPILERS="gcc gfortran"

# Libraries (Eigen, Scipy, BLAS/LAPACK, NLopt)
NEWLIBS="libeigen3-dev libblas-dev liblapack-dev  python-numpy python-scipy python-matplotlib python3-pip libnlopt-dev"

# MPI/Beowulf
NEWMPI="nfs-common nfs-kernel-server openmpi-bin openmpi-doc libopenmpi-dev openssh-server"

# Miscellaneous 
NEWMISC="cmake git htop geany vim gnuplot"

echo "Installing: $NEWCOMPILERS $NEWLIBS $NEWMPI $NEWMISC"

# Install programs listed above
sudo apt-get install $NEWCOMPILERS $NEWLIBS $NEWMPI $NEWMISC -y


echo "Generating ssh-key.."

# Generate ssh key for github using cambridge email address
ssh-keygen -t rsa -b 4096 -C "mcc74@cam.ac.uk" 