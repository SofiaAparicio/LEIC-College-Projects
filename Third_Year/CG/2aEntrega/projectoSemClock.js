var camera, renderer, scene,nave, startTime;
var ortoOn = true;
var viewSize = 900;
var aspectRatio = window.innerWidth/window.innerHeight;
var widthPlan = viewSize * 1.5;
var heigthPlan = viewSize/1.5;
var speed = 0;
var aceleration = 500;
var aceleration2 = 5*aceleration;
var keyDown = false;
var keyAdown = true;

// substituir o traverse por um for e guardar referencias para os aliens e nave

function init(){
	createRenderer();
	createScene();
	createOrtoCamera();
	render();

	window.addEventListener("keydown",onKeyDown);
	window.addEventListener("keyup",onKeyUp);
}

function createRenderer(){
	renderer = new THREE.WebGLRenderer({antialias:true});
	renderer.setSize(window.innerWidth,window.innerHeight);
	document.body.appendChild(renderer.domElement);
	
}

function createOrtoCamera(){
	camera = new THREE.OrthographicCamera(- (viewSize * aspectRatio)/2, (viewSize * aspectRatio)/2,-viewSize/2, viewSize/2,-1,1000); 
	camera.position.x = 0;
	camera.position.y = 500;				//manipula a posicao da camera afetando o modo com se ve a cena
	camera.position.z = 0;
	camera.lookAt(scene.position);
}

function createPerspCamera(){
	camera = new THREE.PerspectiveCamera(70, window.innerWidth / window.innerHeight,1,1000);
	camera.position.x = 200;
	camera.position.y = 200;
	camera.position.z = 500;
	camera.lookAt(scene.position);
}

function render(){
	renderer.render(scene,camera);
}


function createScene(){
	scene = new THREE.Scene();
	scene.add(new THREE.AxisHelper(100));
	createAlienPlan();	
	addAliens();
	createShip();
}

function createAlienPlan(){   //cria o plano onde vao estar os aliens
	var geometry = new THREE.PlaneGeometry(widthPlan,heigthPlan);
	var material = new THREE.MeshBasicMaterial({color:0xffffff, wireframe:true});
	var mesh = new THREE.Mesh(geometry,material);
	mesh.position.set(0,-30,90);
	mesh.rotateX(Math.PI/2);
	scene.add(mesh);
}

//----------------------- ALIEN -------------------------------


//---------------- Geometrias do Alien --------------
function addLegs(obj, material, string){
  'use strict';

  var geometry = new THREE.CubeGeometry(15, 30, 10,5,5);
  var leg1 = new THREE.Mesh(geometry, material);
  var leg2 = leg1.clone();
  leg1.position.set(-15, -52, 0);
  leg2.position.set(15,-52, 0); 
  obj.add(leg1);
  obj.add(leg2);
}

function addFeet(obj,material, string){
  'use strict';

  var geometry = new THREE.CubeGeometry(30,10,10,5);
  var foot1 = new THREE.Mesh(geometry, material);
  var foot2 = foot1.clone();
  foot1.position.set(-20, -67, 0);
  foot2.position.set(20,-67,0);
  obj.add(foot1);
  obj.add(foot2);
}

function addEyes(obj,material){ //cria os dois olhos do alien
	var geometry = new THREE.SphereGeometry(10,5,5);
	var material = new THREE.MeshBasicMaterial({color:0xff0000,wireframe:true});
	var eye1 = new THREE.Mesh(geometry,material);
	var eye2 = eye1.clone();
	eye1.position.set(20,50,0);
	eye2.position.set(-20,50,0);
	obj.add(eye1);
	obj.add(eye2);
}

function addArms(obj,material,string){
	var geometry = new THREE.CylinderGeometry( 5, 5, 30, 5,7);
	var arm1 = new THREE.Mesh( geometry, material );
	var arm2 = arm1.clone();
  	arm1.position.set(-50, -15, 0); // -15 pois 15 é metade da altura e o inicio do braço é para ficar alinhado com o centro do alien em y
  	arm2.position.set(50, -15 ,0);
  	obj.add(arm1);
 	obj.add(arm2);
}

function addTorus(obj,material){
  'use strict';

  var geometry = new THREE.TorusGeometry(30,15,16,50);
  var mesh = new THREE.Mesh(geometry, material);
  mesh.position.set(0,0, 0);

  obj.add(mesh);
}

//-------------------------------------------------------

function createAlien(x, y, z){
  'use strict';

  var alien = new THREE.Object3D();
  var material = new THREE.MeshBasicMaterial({color: 0x0000ff, wireframe:true});
  addLegs(alien,material);
  addFeet(alien,material);
  addArms(alien,material);
  addTorus(alien,material);
  addEyes(alien,material);

  alien.position.x = x;
  alien.position.y = y;
  alien.position.z = z;
  alien.rotateX(Math.PI/2);  //rotacao do objecto em relacao ao seu proprio eixo
  scene.add(alien);

 }
 //----------------------------------------------------------------------------


 //---------------------- Criar filas de aliens -------------------------------
 function addAliens(){
 	var x = -widthPlan/2+ widthPlan/5;
 	var z = 90 + heigthPlan/2 - heigthPlan/3;  	// o centro do plano de jogo esta (0,0,90) ; na rotaçao o alien roda em torno do eixo de x com origem no seu centro
 	var y = 0;									// os valores do centro em z (do 2D) correspondem aos valores em y quando pensamos na figura em 3D.
 	var i,j;
 	for( i=1;i<=2;i++){
 		for(j=1; j<=4 ;j++){
 			createAlien(x,y,z);
 			x = x+ widthPlan/5;
 		}
 		z = z - heigthPlan/3;
 		x = -widthPlan/2+ widthPlan/5;
 	}
 }


//---------------------------------------------------------------------------


//----------------------------------- Ship -------------------------------------

function addToship(obj, material, string){
  'use strict';
  	var geometry;
  	var mesh;
 	if (string == "base"){
	  geometry = new THREE.CubeGeometry(100, 15, 10);
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
	  geometry = new THREE.CubeGeometry(10, 15, 10);
	  mesh = new THREE.Mesh(geometry, material);
	  mesh.position.set(0, 30, 0);
	}
  obj.add(mesh);
}

function createShip(){
	 nave = new THREE.Object3D();
 	 var material = new THREE.MeshBasicMaterial({color: 0x0000ff, wireframe:true});
 	 addToship(nave, material, "base");
 	 addToship(nave, material, "middle");
 	 addToship(nave, material, "top");
 	 addToship(nave, material, "gun");

 	 nave.position.x = 0;
 	 nave.position.y = 0;
 	 nave.position.z = -200;
 	 nave.userData = { aceleration:0 ,speed:0, initPos:0};
 	 nave.rotateX (Math.PI/2);
 	 scene.add(nave);

}

//------------------------------------------------------------------------------


//-----------------------Key Down-------------------------------------------
function onKeyDown(tecla){
	switch (tecla.keyCode){
		case 65://A
		case 86://a
			keyAdown = !keyAdown
			scene.traverse(function (node){
				if (node instanceof THREE.Mesh)
					node.material.wireframe = keyAdown;
			});
			render();
			break;
		case 66:
			scene.traverse(function (node){
				if (node instanceof THREE.Object3D)
					console.log(node.rotation.x);
			});
		case 83:
		case 115: //s
			if(ortoOn){
				createPerspCamera();
				render();
				ortoOn = false;
			}
			else{
				createOrtoCamera();
				render();
				ortoOn = true;
			}
			break;
		case 37:  //seta esquerda
			if(nave.userData.aceleration == 0){
				keyDown = true;
				nave.userData.initPos = nave.position.x;
				startTime = new Date().getTime();
				nave.userData.aceleration = -1*aceleration; //troca o sinal da aceleracao
			}
			/*else if(nave.userData.aceleration > 0){
				onKeyUp(39);
			}*/
			break;
		case 39:  //seta direita
			if(nave.userData.aceleration == 0){
				keyDown = true;
				nave.userData.initPos = nave.position.x;
				startTime = new Date().getTime();
				nave.userData.aceleration = aceleration;
			}
			else if(nave.userData.aceleration < 0){
				onKeyUp(37);
			}
			break;
	}
}

//------------------------- Key Up ------------------------------------------
function onKeyUp(tecla){
	switch(tecla.keyCode){
		case 37://seta esquerda
			keyDown = false;
			nave.userData.initPos = nave.position.x;
			nave.userData.speed = speed;
			startTime = new Date().getTime();
			nave.userData.aceleration = aceleration2;
			break;
		case 39://seta direita
			keyDown = false;
			nave.userData.initPos = nave.position.x;
			nave.userData.speed = speed;
			startTime = new Date().getTime();
			nave.userData.aceleration = -1*aceleration2;
			break;
	}
}

//---------------------------------------------------------------------------

function shipMovement(){
	var timePassed = new Date().getTime() - startTime;
	if (keyDown ||(nave.userData.aceleration < 0 && speed > 0) || (nave.userData.aceleration > 0 && speed < 0)){ //keyUp
			nave.position.x = nave.userData.initPos + nave.userData.speed*timePassed*0.001 + (nave.userData.aceleration/2)*Math.pow(timePassed*0.001,2);
			speed = nave.userData.speed + nave.userData.aceleration*timePassed*0.001;
		}
	else{
		nave.userData.aceleration = 0;
		nave.userData.speed = 0;
	}
	render();
	requestAnimationFrame(shipMovement);
}