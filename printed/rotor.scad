$fn=50;
translate([0,0,7])cylinder(h=4,r=1.8,center=true);
difference(){
	cylinder(h=13,r=4.5,center=true);
	translate([0,0,-3])cylinder(h=8,r=1.6,center=true);

	rotate([0,90,0]){
	for ( i = [0 : 5] ){
	   rotate( i * 360 / 6, [90, 0, 0])
   	translate([-1, 7.7, 0])cylinder(r=5,h=2.8,center=true);
	}
	}
}