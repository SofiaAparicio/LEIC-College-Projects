class GameObject extends THREE.Object3D{

	constructor(x,y,z,speedx,speedz,raio,mesh){
		super();
		this.speedx = speedx;    //definir atributos speed e aceleracao
		this.speedz = speedz;
		this.acelerationx = 0;
		this.acelerationz = 0;

		this.auxPosX = x;
		this.auxPosZ = z;
 		this.raio = raio;
 		this.dead = false;

		this.position.x = x;
	  	this.position.y = y;
	  	this.position.z = z;

	  	var boundingVolume = new THREE.Object3D();  //para ao carregarmos no espaco mostrarmos a bounding esfera
	  	var material = new THREE.MeshBasicMaterial({color:0xffffff, wireframe:true})
	  	var geometry = new THREE.SphereGeometry(raio,20,20)
	  	var mesh = new THREE.Mesh(geometry,material);
	  	boundingVolume.add(mesh);
	  	boundingVolume.visible = false;
	  	boundingVolume.position.set(0,0,0);
	  	this.add(boundingVolume);
	  	this.boundingVolume = boundingVolume;
	}
	//------------------------ Funcoes para colisoes ----------------------
	update(timePassed){
		this.speedz = this.speedz + this.acelerationz*timePassed;
		this.speedx = this.speedx + this.acelerationx*timePassed;
		this.auxPosX = this.position.x + this.speedx*timePassed;
		this.auxPosZ = this.position.z + this.speedz*timePassed;
	}
	setPosition(){
		this.position.x = this.auxPosX;
		this.position.z = this.auxPosZ;
	}

	colidesWith(string){
		if(string == "width"){
			return ((this.auxPosX + this.raio > widthPlan/2)||(this.auxPosX-this.raio < -widthPlan/2))
		}
		else if(string == "height"){
			return ((this.auxPosZ-this.raio < -heightPlan/2 ) || (this.auxPosZ +this.raio > heightPlan/2))
		}
	}
	hasColision(object){
		if(Math.pow(this.raio+object.raio,2)>= (Math.pow(this.auxPosX-object.auxPosX,2)+
			Math.pow(this.auxPosZ-object.auxPosZ,2))){
			return true;
		}
		return false;
	}
	treatColision(object){}

	newPos(){}
	//-----------------------------------------------------------------------

}


/*----------------------------------- ALIEN -----------------------------------*/

class Alien extends GameObject{

	constructor(x, y, z,material,eyeMaterial){
	  'use strict';


	  super(x,y,z,0,0,50);   //(x,y,z,speedx,speedz,raio)
	  this.addLegs(material);
	  this.addFeet(material);
	  this.addArms(material);
	  this.addTorus(material);
	  this.addEyes(eyeMaterial);

	}

	addLegs(material){
	  'use strict';

	  var geometry = new THREE.CubeGeometry(15, 30, 10,5,5); //(width,height,depth,heightSegments,widthSegments)
	  var leg1 = new THREE.Mesh(geometry, material);
	  var leg2 = leg1.clone();
	  leg1.position.set(-15, -45, 0);
	  leg2.position.set(15,-45, 0);
	  this.add(leg1);
	  this.add(leg2);
	}

	addFeet(material){
	  'use strict';

	  var geometry = new THREE.CubeGeometry(30,10,10,5);//(width,height,depth,heightSegments,widthSegments)
	  var foot1 = new THREE.Mesh(geometry, material);
	  var foot2 = foot1.clone();
	  foot1.position.set(-20, -60, 0);
	  foot2.position.set(20,-60,0);
	  this.add(foot1);
	  this.add(foot2);
	}

	addEyes(material){ //cria os dois olhos do alien
		var geometry = new THREE.SphereGeometry(10,5,5);  //(radius, with segments,heigth segments)
		var eye1 = new THREE.Mesh(geometry,material);
		var eye2 = eye1.clone();
		eye1.position.set(20,45,0);
		eye2.position.set(-20,45,0);
		this.add(eye1);
		this.add(eye2);
	}

	addArms(material){
		var geometry = new THREE.CylinderGeometry( 5, 5, 35, 5,7);   //(radiusTop, radiusBottom,height, radiusSegments,heightSegments)
		var arm1 = new THREE.Mesh( geometry, material );
		var arm2 = arm1.clone();
	  	arm1.position.set(45, -15, 0); // -15 pois 15 é metade da altura e o inicio do braço é para ficar alinhado com o centro do alien em y
	  	arm2.position.set(-45, -15 ,0);
	  	this.add(arm1);
	 	this.add(arm2);
	}

	addTorus(material){
	  'use strict';

	  //var geometry = new THREE.TorusGeometry(30,15,15,30); //(radius, tube, radialSegments, tubularSegments)
	  var geometry = new THREE.SphereGeometry(40,15,15);
	  var mesh = new THREE.Mesh(geometry, material);        //tube = diametro do tubo
	  mesh.position.set(0,0, 0);

	  this.add(mesh);
	}

   //------------------------- Funcoes para Colisoes ---------------------------
	treatColision(object){
		if(object instanceof Bullet){
			this.dead = true;
		}
		
		else{
			this.speedx = -this.speedx;
			this.speedz = -this.speedz;
			this.auxPosX = this.position.x;
			this.auxPosZ = this.position.z;
		}

	}
	newPos(){
		if(this.colidesWith("width")){//limite direito do movimento
			this.speedx = -this.speedx;
		}
		else if (this.colidesWith("height")){ //limite em baixo do movimento dos aliens
			this.speedz = - this.speedz;
		}
		else{
			this.setPosition()
		}
	}
	//-----------------------------------------------------------------------

}
/*------------------------------------------------------------------------------*/


//----------------------------------- Ship -------------------------------------

class Ship extends GameObject{

	constructor(material,camera){
	 	super(0,0,350,0,0,50);  //(x,y,z,speedx,speedz,raio)
	 	this.addToship(material, "base");
	 	this.addToship(material, "middle");
	 	this.addToship(material, "top");
	 	this.addToship(material, "gun");
		//this.add(new THREE.AxisHelper(100))
	 	this.add(camera);
	 	this.rotateX (-Math.PI/2);

	}

	addToship(material, string){
	  'use strict';
	  	var geometry;
	  	var mesh;
	 	if (string == "base"){
		  geometry = new THREE.CubeGeometry(100, 15, 10); //(width,height,depth,heightSegments,widthSegments)
		  mesh = new THREE.Mesh(geometry, material);
		  mesh.position.set(0, -15, 0);
		}
		else if (string == "middle"){
		  geometry = new THREE.CubeGeometry(70, 15, 10);
		  mesh = new THREE.Mesh(geometry, material);
		  mesh.position.set(0, 0, 0);
		}
		else if (string == "top"){
		  geometry = new THREE.CubeGeometry(30, 15, 10);
		  mesh = new THREE.Mesh(geometry, material);
		  mesh.position.set(0, 15, 0);
		}
		else{
		  geometry = new THREE.CubeGeometry(10, 30, 10);
		  mesh = new THREE.Mesh(geometry, material);
		  mesh.position.set(0, 35, 0);
		}
	  	this.add(mesh);

	}
	//-------------------------- Funcoes para colisoes -------------------------
	newPos(){
		if(this.colidesWith("width")){//limite direito do movimento
			this.acelerationx = 0;
			this.speedx = 0;
			if(this.auxPosX > 0){
				this.position.x = widthPlan/2-50; //50 vem da largura da nave
			}
			else{
				this.position.x = -widthPlan/2+50;
			}
		}
		else if ((this.acelerationx == acelerationx2 && this.speedx > 0) ||
			(this.acelerationx == -acelerationx2 && this.speedx < 0)){
				this.acelerationx = 0;
				this.acelerationz = 0;
				this.speedx = 0;
		}
		else{
			this.setPosition()
		}
	}
	//-------------------------------------------------------------------------
}

//-------------------------------------------------------------------------------------

//------------------------------------ BULLET -----------------------------------------
class Bullet extends GameObject{
	constructor(x,y,z){
		var material = new THREE.MeshBasicMaterial({color:0xffffff, wireframe:true})
		var geometry = new THREE.SphereGeometry(5,20,20)
		var mesh = new THREE.Mesh(geometry,material);
		super(x,y,z,0,-500,5); //(x,y,z,speedx,speedz,raio)
		this.add(mesh);
	}
	//-------------------------- Funcoes para colisoes ----------------------

	treatColision(object){
		this.dead = true;
	}

	newPos(){
		if(this.colidesWith("height")){
			this.dead = true;
		}
		else{
			this.setPosition()
		}
	}
}
