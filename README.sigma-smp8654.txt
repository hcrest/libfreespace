How to build libfreespace and examples on the Simga 8654

A. Building libusb-1.0 (required by libfreespace)
1) Download the libusb-1.0 source from Sourceforge.
2) Set the PATH variable to include the path to the 8654 mipsel-linux-gcc:    
        export PATH=<path to Sigma compilers>/smp86xx-R3.6/bin:$PATH

3) Set CFLAGS to include "-msoft-float":    
        export CFLAGS="-msoft-float"
    
4) Set LIBS to link with libpthread.  There's no pthread dependency specified in libusb's configure.ac.  libusb links correctly with libpthread when building natively but not when cross compiling:   
        export LIBS="-lpthread"
      
5) Run "configure" and "make" with the appropriate host and prefix arguments:    
        ./configure --host=mipsel-linux --prefix=<install path>

B. Building libfreespace (requires libusb-1.0)
1) Set the PATH variable to point to the location of mipsel-linux-gcc:    
        export PATH=/usr/local/compilers/smp86xx-R3.6/bin:$PATH

2) Generate the configure script if it's not present:
        autoreconf -v --install
        
3) Set CFLAGS to include "-msoft-float" and the path to the libusb headers:
        export CFLAGS="-msoft-float -I<libusb install path>/include" 
      
4) Set LDFLAGS to include the path to the libusb libraries:
        export LDFLAGS="-L<libusb install path>/lib"

5) Run "configure" and "make" with the appropriate host and prefix arguments:    
        ./configure --host=mipsel-linux --prefix=<install path>
        
C. Building libfreespace-examples (requires libfreespace)
1) Set the PATH variable to point to the location of mipsel-linux-gcc:    
        export PATH=/usr/local/compilers/smp86xx-R3.6/bin:$PATH

2) Generate the configure script if it's not present:
        autoreconf -v --install

3) Set CFLAGS to include "-msoft-float", the include  path to the libfreespace headers, and -rpath-link to the libfreespace libraries:
        export CFLAGS="-msoft-float -I<libfreespace install path>/include\
            -Wl,-rpath-link,<libfreespace install path>/lib" 
            
   The -rpath-link must be set before configuring or buildng because ld cannot resolve recursive dependencies when cross compiling.
        
4) Set LDFLAGS to include the path to the libfreespace libraries:
        export LDFLAGS="-L<libfreespace install path>/lib"
        
5) Run "configure" and "make" with the appropriate host and prefix arguments:    
        ./configure --host=mipsel-linux --prefix=<install path>
        
Troubleshooting:
-If you have trouble running autoreconf or configure, you may need a newer version of autoconf and it's dependencies (libtool and m4).


David Tucker <David.Tucker@hillcrestlabs.com>
