
$fn=50;

//основа
translate([0,5,-24]){
translate([-4,0,2])rotate([180,90,0])cylinder(h=50,r=2);
translate([54,0,2])rotate([180,90,0])cylinder(h=50,r=2);

difference(){

hull(){
	translate([30,0,0])cylinder(h=2,r=25);
	translate([-30,0,0])cylinder(h=2,r=25);
}
	translate([0,-5,0])cylinder(h=2,r=5);

}
}



difference(){
	union(){
		//большой конус
		hull(){
			cylinder(r=5,h=1);
			translate([0,0,45])cylinder(r=15,h=1);}
		
		//нижний конус
		hull(){
			translate([0,0,-22])cylinder(h=1,r=7);
			translate([0,0,0])cylinder(h=1,r=5);}

		//цилиндр корпус
		rotate([90,0,0])cylinder(h=15,r=8,center=true);
		
		//крепление мотора
		translate([25,8.5,0])rotate([90,180,0])motor_couping();
		}

	
	//большая конусная выемка
	translate([0,0,0]){
	hull(){
	cylinder(r=4,h=1);
	translate([0,0,45])cylinder(r=14,h=1);
	}

	//малая конусная выемка

	hull(){
	translate([0,0,-22])cylinder(h=1,r=5);
	translate([0,0,0])cylinder(h=1,r=3.9);
	}

	//цилиндр выемка
	translate([0,2,0])rotate([90,0,0])cylinder(h=15,r=5,center=true);

	//маленькое отверстие
	rotate([90,0,0])cylinder(h=15,r=1.5,center=true);

	}
}


module motor_couping(){
difference(){
	union(){
	hull(){
		cylinder(r=5,h=2);
		translate([50,0,0])cylinder(r=5,h=2);
		translate([25,0,])cylinder(r=21,h=2);

		cube([50,22,2]);

	}
		cylinder(r=3,h=5);
		translate([50,0,0])cylinder(r=3,h=5);
	}

	cylinder(r=1,h=6);
	translate([50,0,0])cylinder(r=1,h=6);
	translate([25,0,0])cylinder(r=6.5,h=2);
}
}