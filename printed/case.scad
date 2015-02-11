$fn=50;

x=102.8;
y=74.4;
z=26;
l=1.6;


difference(){
union(){

//support brim
//translate([-5,-5,0])difference(){cube([x+10,y+10,0.6]);}

cube([x,y,z]);
}
translate([l,l,l])cube([x-l*2,y-l*2,25]);

//экран
translate([(x/2)-72/2-3.2,y-(13+25),0])cube([72,24.8,l+0.1]);

//кнопки
translate([14,8,0])button();
translate([14,16,0])button();

translate([24,8,0])button();
translate([24,16,0])button();

translate([34,8,0])button();
translate([34,16,0])button();

//разьем питания
translate([0,l+4.5,l+8])cube([l+0.1,9.5,12]);

//дырка под провода для двигателя
translate([x-l-1,y/2-5,15])rotate([0,90,0])cylinder(r=3,h=10);

//дырка под провода для двигателя
translate([x-l-1,y/2+10,15])rotate([0,90,0])cylinder(r=2.5,h=10);
}

//столбики под клаву
translate([10,5,0])cylinder(h=7,r=3);
translate([10,5,0])cylinder(h=11,r=1);

translate([10,27,0])cylinder(h=7,r=3);
translate([10,27,0])cylinder(h=11,r=1);

translate([46,5,0])cylinder(h=7,r=3);
translate([46,5,0])cylinder(h=11,r=1);

translate([46,27,0])cylinder(h=7,r=3);
translate([46,27,0])cylinder(h=11,r=1);

//столбики под часы
translate([52,26,0])cylinder(h=3,r=3);
translate([52,26,0])cylinder(h=7,r=1);

translate([52,8,0])cylinder(h=3,r=3);
translate([52,8,0])cylinder(h=7,r=1);

translate([52+37,26,0])cylinder(h=3,r=3);
translate([52+37,26,0])cylinder(h=7,r=1);

translate([52+37,8,0])cylinder(h=3,r=3);
translate([52+37,8,0])cylinder(h=7,r=1);

//столбики под экран
translate([11,33,0])cylinder(h=7,r=3);
translate([86,33,0])cylinder(h=7,r=3);
translate([11,64,0])cylinder(h=7,r=3);
translate([86,64,0])cylinder(h=7,r=3);

translate([10.5,33,0])cylinder(h=10,r=1);
translate([85.5,33,0])cylinder(h=10,r=1);
translate([10.5,64,0])cylinder(h=10,r=1);
translate([85.5,64,0])cylinder(h=10,r=1);

//подпорки под плату

translate([10,y-5,l])cube([l,5,20]);
translate([x-20-l,y-5,l])cube([l,5,20]);

hull(){
	translate([x-20-l,l,l])cube([l,1,1]);
	translate([x-20-l,l,l+19])cube([l,5,1]);
}


//столбики для крепления крышки 

//translate([1,1,1])stolbik();
translate([x-l,1,1])rotate([0,0,90])stolbik();
//translate([l,y-l,1])rotate([0,0,270])stolbik();
translate([x-l,y-l,1])rotate([0,0,180])stolbik();

translate([l,39,0]){
difference(){
	hull(){
	cube([1,8,1]);
	translate([0,0,20])cube([8,8,1]);
	}

translate([4,4,2])cylinder(h=30,r=1);
}
}


module stolbik(){
difference(){

hull(){
translate([0,0,22])cube([8,8,1]);
cube([1,1,1]);
}

translate([4,4,2])cylinder(h=30,r=1);
}
}

module button(){
	cube([8,6,l+0.1]);
}



