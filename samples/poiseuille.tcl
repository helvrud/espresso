setmd box_l 32 32 32
set dt 0.01
setmd time_step $dt
setmd skin 0.3

lbboundary wall dist 1.0 normal 0.0 1.0 0.0
lbboundary wall dist -31.0 normal 0.0 -1.0 0.0

#should give two plane walls effectively at y=1 and y=31.
#This will result in boundary nodes at y=0.5 and y=30.5.

# set the fluid  
#note that the time step only needs to be larger than the md time step 
lbfluid agrid 1.0 dens 1.0 visc 3.0 tau $dt ext_force 0.001 0.0 0.0 friction 20.0
#note that adding the thermostat slows the simulation significantly and thermostat lb 0.0 actually does nothing
#more appropriate would be thermostat off but thermostat lb 0.0 is left in as an example of how to set the thermostat
thermostat lb 0.0

#here we print out the boundaries once at the start and the fluid over time in vtk format, ideal for Paraview
lbfluid print vtk boundary lb_fluid_boundary.vtk
for {set i 0} {$i<100} {incr i} {
 lbfluid print vtk velocity lb_fluid_vel_frame_$i.vtk
 integrate 100
}
