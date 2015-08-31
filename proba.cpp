
/*Bookmark the permalink.
Installing Open MPI 1.6.5 (Ubuntu 12.04, 13.04, fedora)
Nov4 by mattreid9956

The OpenMPI logo. A box filled with useful tricks!
Open MPI (http://www.open-mpi.org )  is one of the most liberating tools out there. In a world where time is of the essence and most computers these days have more than one core, why let them sit around idle doing nothing, put them to good use!!

It stands for Message Parsing Interface (MPI) and allows you to do a whole manner of parallelised computing applications. For an in-depth idea of what it can do visit the website but a few key things that are useful to know are:

    Collective functions: Main example is the MPI Reduce function, which allows you to perform simple operations such as a summation using the family of processors to do so.
    Point-to-Point communication: Most common example I can think of is the use of a head node splitting a dataset into smaller memory chunks based on the number of sub-processors that are available, where each will then compute the same task in parallel. This operations protocol is usually called a master-to-slave process.
    Communicators: These connect all the MPI processes into groups, called process groups. An example is the world communicator which contains attributes such as the size (number of processors) and rank (ordered topology) of the group. They also allow for the manipulation of process groups.

How to Install
Open MPI is relatively simple to install, I should point out I have not tried this on a Fedora machine but as long as you have the same libraries/dependencies then it should be the same procedure. There are two methods; the first (and not my preferred method!) is to simply find the latest version from the on-line repositories as of today the latest version was 1.6.5. To do this you can
sudo apt-get update
sudo apt-get install -y autotools-dev g++ build-essential openmpi1.5-bin openmpi1.5-doc libopenmpi1.5-dev
# remove any obsolete libraries
sudo apt-get autoremove

That should get you what you need, if you are on Fedora simply “sudo yum search openmpi” that should bring you up something similar to openmpi, openmpi-devel. The next method and my preferred way as you get the most recent update version(currently 1.6.5) is to take it directly from the website. The below script worked for me on Ubuntu 13.04 tested on 25/07/2013.

The script below will install Open MPI in your /usr/local area, this can be modified by changing the parameter installDIR in the script to the desired location. After install the libraries are placed in $installDIR/lib/openmpi and you can now begin playing with Open MPI. One thing to note is that I apply ldconfig to the /usr/local/lib, this is a much better method than setting paths explicitly. To do this you need to modify your ld.so.conf.d directory to make it look in the /usr/local/xxx area if it doesn’t already. Now with Ubuntu you may already have this linked up so check if your machine has a file called “/etc/ld.so.conf.d/libc.conf” and a path explicitly showing “/usr/local/lib” and that should be all. If you do then you can ignore this step, else you can add the path using the following:
sudo echo "/usr/local/lib" >> /etc/ld.so.conf.d/local.conf
sudo ldconfig -v

I prefer this method as you do not have to keep adding things to your LD_LIBRARY_PATH all the time which is not really recommended, see http://www.xahlee.info/UnixResource_dir/_/ldpath.html for a couple of examples for the case against setting this path. I should have mentioned this in previous blogs too!!
# Matthew M Reid. install open mpi shell script

# install destination
installDIR="/usr/local"
# First get necessary dependencies
sudo apt-get update
sudo apt-get install -y gfortran autotools-dev g++ build-essential autoconf automake
# remove any obsolete libraries
sudo apt-get autoremove

# Build using maximum number of physical cores
n=`cat /proc/cpuinfo | grep "cpu cores" | uniq | awk '{print $NF}'`

# grab the necessary files
wget http://www.open-mpi.org/software/ompi/v1.6/downloads/openmpi-1.6.5.tar.gz
tar xzvf openmpi-1.6.5.tar.gz
cd openmpi-1.6.5

echo "Beginning the installation..."
./configure --prefix=$installDIR
make -j $n
make install
# with environment set do ldconfig
sudo ldconfig
echo
echo "...done."

So finally to test the installation here is a simple example that just prints out some information from each processor.*/
#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int numprocessors, rank, namelen;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocessors);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(processor_name, &namelen);

    if ( rank == 0 )
    {
        std::cout << "Processor name: " << processor_name << "\n";
    std::cout << "master (" << rank << "/" << numprocessors << ")\n";
    } else {
        std::cout << "slave  (" << rank << "/" << numprocessors << ")\n";
   }
   MPI_Finalize();
   return 0;
}
