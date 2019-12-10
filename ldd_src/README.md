# ldd src

<u3>Useful Commands:</h3>
<ul>
<li>sudo make -C /lib/modules/$(uname -r)/build M=$PWD clean</li>
<li>sudo make -C /lib/modules/$(uname -r)/build M=$PWD modules</li>
<li>sudo insmod ./your_driver_name.ko</li>
<li>sudo insmod ./your_driver_name.ko name="Your string param" count=20 is_formal=0</li>
<li>sudo insmod ./your_driver_name.ko major=0 usr_name="Your string param" usr_params=10,20</li>
<li>sudo rmmod your_driver_name</li>
<li>lsmod</li>
<li>tail -f /var/log/syslog</li>
<li>cat /proc/devices</li>
<li>sudo ls -l /sys/module/your_driver_name/parameters/</li>
<li>sudo ls -l /sys/module/your_driver_name/parameters/param_name</li>
<li>sudo cat /sys/module/your_driver_name/parameters/param_name</li>
<li>echo 10 | sudo tee /sys/module/your_driver_name/parameters/param_name</li>
</ul>



<h3>My References:</h3>

https://www.kernel.org/doc/Documentation/ <br />
https://lwn.net/Kernel/LDD3/ <br />
http://www.embeddedgyan.com/ <br />
https://www.youtube.com/watch?v=Zn8mEZXr6nE&t=355s <br />
https://www.youtube.com/watch?v=IiHrZcZrT64 <br />
https://www.youtube.com/watch?v=78YkQ4KBkjA <br />
https://www.youtube.com/watch?v=VtuA30f_ZuA <br />
https://www.youtube.com/watch?v=mC20o-3olYs <br />
https://www.youtube.com/watch?v=E_xrzGlHbac <br />
https://www.researchgate.net/figure/Linux-User-and-Kernel-space_fig1_245022829 <br />
https://www.tldp.org/LDP/lkmpg/2.4/html/c43.html <br />
https://www.fsl.cs.sunysb.edu/kernel-api/ <br />
https://www.gnu.org/software/libc/manual/html_node/Access-Modes.html <br />
https://www.youtube.com/watch?v=U7QFcpVh248&list=PL1zwAXk5ZrWKxLyCq73lzFn3oSLruM750 <br />
https://www.youtube.com/watch?v=VXIFibjNLhU&list=PL1zwAXk5ZrWKxLyCq73lzFn3oSLruM750&index=2 <br />
https://www.youtube.com/watch?v=1v_ALs6zuRw&list=PL1zwAXk5ZrWKxLyCq73lzFn3oSLruM750&index=3 <br />
https://www.youtube.com/watch?v=kpKZPBUepxo&list=PL1zwAXk5ZrWKxLyCq73lzFn3oSLruM750&index=4 <br />
https://www.youtube.com/watch?v=RwRje_Vy2u4&list=PL1zwAXk5ZrWKxLyCq73lzFn3oSLruM750&index=5 <br />
https://www.youtube.com/watch?v=zTTzIc1zJ3s&list=PL1zwAXk5ZrWKxLyCq73lzFn3oSLruM750&index=6 <br />
https://www.youtube.com/watch?v=Sp0vfWll7H8&list=PL1zwAXk5ZrWKxLyCq73lzFn3oSLruM750&index=7 <br />
https://www.youtube.com/watch?v=sen-FlxJu_M&list=PL1zwAXk5ZrWKxLyCq73lzFn3oSLruM750&index=8 <br />
https://www.youtube.com/watch?v=_k6zg5mLDXk&list=PL1zwAXk5ZrWKxLyCq73lzFn3oSLruM750&index=9 <br />
https://www.youtube.com/watch?v=3ccMOOLS6xY&list=PL1zwAXk5ZrWKxLyCq73lzFn3oSLruM750&index=10 <br />
https://linux-kernel-labs.github.io/master/labs/device_drivers.html <br />
http://opensourceforu.com/tag/linux-device-drivers-series/<br />

