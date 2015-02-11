//translate([6.5,29.5,40])rotate([0,0,0])

keyboard();

module c(){
	cube([3,3,1]);
}
module keyboard(){

	y1=8;
	y2=16;
	y3=0;

	difference(){
	union(){
		cube([42,20,0.6]);
		translate([11,-8,0])cube([20,35,0.6]);
	}

	translate([7,y1,0])c();
	translate([13,y1,0])c();
	translate([17,y1,0])c();
	translate([23,y1,0])c();
	translate([27,y1,0])c();
	translate([34,y1,0])c();

	translate([7,y2,0])c();
	translate([13,y2,0])c();
	translate([17,y2,0])c();
	translate([23,y2,0])c();
	translate([27,y2,0])c();
	translate([34,y2,0])c();

	translate([7,y3,0])c();
	translate([13,y3,0])c();
	translate([17,y3,0])c();
	translate([23,y3,0])c();
	translate([27,y3,0])c();
	translate([34,y3,0])c();


	}
	kx=4;
	ky=-1;

	translate([kx+4,ky+4,0])but();
	translate([kx+5.4,ky+6.5,4.1])trg();


	translate([kx+4,ky+12,0])but();
	translate([kx+26,ky+12.5,3.9])rotate([0,0,4])cube([1,3,0.6]);

	translate([kx+26.6,ky+12,3.9])rotate([0,0,45])cube([5,1,0.6]);


	translate([kx+14,ky+4,0])but();
	translate([kx+17.4,ky+4.5,4.1])rotate([0,0,90])trg();

	translate([kx+14,ky+12,0])but();
	translate([kx+17.4,ky+16.5,4.1])rotate([0,0,270])trg();

	translate([kx+24,ky+4,0])but();
	translate([kx+29.4,ky+6.5,4.1])rotate([0,180,0])trg();
	translate([kx+24,ky+12,0])but();
	translate([kx+6.5,ky+13,3.9])rotate([0,0,45])x();

}

module but(){
	cube([7,5,4]);
}

module trg(){
	
	hull(){
	cube([0.1,0.1,0.3],center=true);
	translate([4,0,0])cube([0.1,4,0.6],center=true);
	}
}



module x(){

	cube([4,1,0.6]);
	translate([1.5,-1.5,0])cube([1,4,0.6]);


}