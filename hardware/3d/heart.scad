
module arduino()
{
    color("blue")
    cube (size=[100,50,2]);
    color("black")
    translate([0,0,-10])
    {
        translate([3,0,0])
    cube (size=[5,50,10]);
        translate([10,3,0])
    cube (size=[70,2,10]);
        translate([10,46,0])
    cube (size=[70,2,10]);
    }
}

//cube (size=[150,150,2]);

module heart()
{
    color("red")
    linear_extrude(height = 2, center = false, convexity = 10)
    {
import (file = "pcb/aux/heart.dxf", layer = "0");
//import (file = "aux/heart.dxf", layer = "gambe");
    }
        
translate([25,65,12])
    arduino();

}

module stand()
{
    translate([-168,0,0])
    color("green")
    linear_extrude(height = 2, center = false, convexity = 10)
    {
import (file = "pcb/aux/heart.dxf", layer = "stand");
    }
    
    translate([80,0,-35])
    cube(size=[2,50,70]);
}

rotate([15,0,0]) heart();
translate([75,0,0]) rotate([0,-90,0]) stand();