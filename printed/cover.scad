$fn=50;
difference(){
cube([100,71,2]);
translate([4,41,0])cylinder(h=2,r=1.5);
translate([96,4,0])cylinder(h=2,r=1.5);
translate([96,67,0])cylinder(h=2,r=1.5);

translate([4,41,1])cylinder(h=2,r=2);
translate([96,4,1])cylinder(h=2,r=2);
translate([96,67,1])cylinder(h=2,r=2);
}

translate([15,55,0])lapa();
translate([70,55,0])lapa();

module lapa(){
cube([15,10,6]);

difference(){
hull(){
translate([0,0,6])cube([15,10,3]);
translate([7.5,-35,6])cylinder(h=3,r=5);
}
translate([7.5,-35,6])cylinder(h=3,r=2);
translate([7.5,-25,6])cylinder(h=3,r=2);
translate([7.5,-15,6])cylinder(h=3,r=2);
translate([7.5,-5,6])cylinder(h=3,r=2);
}
}

