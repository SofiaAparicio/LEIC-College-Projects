var camera,cameraOrto,cameraPerspStatic,cameraPerspShip, renderer, scene,nave;
var heightPlan= 900;
var widthPlan = 1300;
var viewSize = 1000;
var alienSpeed = 100;
var campAspect = widthPlan/heightPlan;
var keyPressed = 0;
var visible = false;
var acelerationx = 1000;
var acelerationx2 = 2*acelerationx;
var materials = []
var gameObjects = []

function init(){
	createRenderer();
	createPerspShipCamera();
	createClock();
	createScene();
	createOrtoCamera();
	camera = cameraOrto;
	createPerspStaticCamera();
	render();

	window.addEventListener('resize',updateCamera);
	window.addEventListener("keydown",onKeyDown);
	window.addEventListener("keyup",onKeyUp);

}

function createRenderer(){
	renderer = new THREE.WebGLRenderer({antialias:true});
	renderer.setSize(window.innerWidth,window.innerHeight);
	document.body.appendChild(renderer.domElement);

}

function render(){
	renderer.render(scene,camera);
}

//------------------------- Cameras ------------------------------------------

function createOrtoCamera(){
	var aspectRatio = window.innerWidth/window.innerHeight;
	if (aspectRatio > campAspect){
		cameraOrto = new THREE.OrthographicCamera(- (viewSize* aspectRatio)/2, (viewSize * aspectRatio)/2,
												viewSize/2, -viewSize/2,
												1,1000);
	}
	else{
		cameraOrto = new THREE.OrthographicCamera(-(viewSize*campAspect)/2, (viewSize*campAspect)/2,
												(viewSize*campAspect/aspectRatio)/2, (-viewSize*campAspect/aspectRatio)/2,
												1,1000);
	}
	cameraOrto.position.x = 0;
	cameraOrto.position.y = 500;				//manipula a posicao da camera afetando o modo com se ve a cena
	cameraOrto.position.z = 0;
	cameraOrto.up.set( 0, 0, -1 );				// manipula o vector up da camera
	cameraOrto.lookAt(scene.position);
}

function createPerspStaticCamera(){
	cameraPerspStatic = new THREE.PerspectiveCamera(100, window.innerWidth / window.innerHeight,1,1000);
	// fov(field of view), aspect, near, far
	cameraPerspStatic.position.x = 0;
	cameraPerspStatic.position.y = 200;
	cameraPerspStatic.position.z = 550;
	cameraPerspStatic.lookAt(scene.position);
}

function createPerspShipCamera(){
	cameraPerspShip = new THREE.PerspectiveCamera(120, window.innerWidth / window.innerHeight,1,1000);
	// fov(field of view), aspect, near, far
	cameraPerspShip.position.set(0,-40,60);
	cameraPerspShip.lookAt(new THREE.Vector3(0, 100,0));
	cameraPerspShip.up = new THREE.Vector3(0, 0,1);
}

//------------------------- Update Cameras --------------------------------

function updateCamera(){
	if(window.innerWidth>0 && window.innerHeight>0){
		renderer.setSize(window.innerWidth,window.innerHeight);
		if (camera === cameraOrto){
			var newAspectRatio = window.innerWidth/window.innerHeight;
			if (newAspectRatio > campAspect){
				camera.left = (viewSize * newAspectRatio)/(-2);
				camera.right = (viewSize * newAspectRatio)/2;
				camera.bottom = (viewSize)/(-2);
				camera.top = (viewSize)/2;
			}
			else{
				// viewSize*campAspect é como se fosse o viewSize 2
				camera.left = (viewSize*campAspect)/(-2);  //para a camera ficar com a proporcao do campo de jogo em termos de largura
				camera.right = (viewSize*campAspect)/2;
				camera.bottom = (viewSize * campAspect/ newAspectRatio)/(-2);
				camera.top = (viewSize * campAspect/ newAspectRatio)/2;
			}
		}
		else{
			camera.aspect = window.innerWidth/window.innerHeight;
		}
		camera.updateProjectionMatrix();
	}
}
//---------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------

function createScene(){
	scene = new THREE.Scene();
	scene.add(new THREE.AxisHelper(100));
	createAlienPlan();
	addAliens();
	createShip();
}

function createAlienPlan(){   //cria o plano onde vao estar os aliens
	var geometry = new THREE.PlaneGeometry(widthPlan,heightPlan);
	var material = new THREE.MeshBasicMaterial({color:0xffffff, wireframe:true});
	var mesh = new THREE.Mesh(geometry,material);
	mesh.position.set(0,-30,0);
	mesh.rotateX(-Math.PI/2);
	scene.add(mesh);
}

function createClock(){
	clock = new THREE.Clock();
}


 //---------------------- Criar filas de aliens -------------------------------
 function addAliens(){
 	var material = new THREE.MeshBasicMaterial({color: 0x4000ff, wireframe:true});
 	materials.push(material)
 	var eyeMaterial = new THREE.MeshBasicMaterial({color:0xff0000,wireframe:true});
	materials.push(eyeMaterial)
 	var x = -widthPlan/2+ widthPlan/5;
 	var z = -90 + heightPlan/2 - heightPlan/3;  // o centro do sitio dos aliens esta (0,0,-90); construi primeiro a linha de baixo
 	var y = 0;									// os valores do centro em z (do 2D) correspondem aos valores em y quando pensamos na figura em 3D.
 	var i,j;									// por causa da rotacao
 	for( i=1;i<=2;i++){							//linhas
 		for(j=1; j<=4 ;j++){					//colunas
 			a = new Alien(x,y,z,material,eyeMaterial);
 			scene.add(a)
			gameObjects.push(a);
 			x = x+ widthPlan/5;
 		}
 		z = z - heightPlan/3;
 		x = -widthPlan/2+ widthPlan/5;
 	}
 }


//---------------------------------------------------------------------------

function createShip(){
	var material = new THREE.MeshBasicMaterial({color: 0x0000ff, wireframe:true});
	materials.push(material)
	nave = new Ship(material,cameraPerspShip);
	scene.add(nave);
	gameObjects.push(nave);
}

function createBullet(){
	var bullet = new Bullet(nave.position.x,nave.position.y,nave.position.z - 50 /*37.5*/);
	gameObjects.push(bullet);
	scene.
	(bullet);
}

//-----------------------Key Down-------------------------------------------
function onKeyDown(tecla){
	switch (tecla.keyCode){
		case 32:
			if(visible){
				visible = false;
				for (e in gameObjects){
					gameObjects[e].boundingVolume.visible = false;
				}
			}
			else{
				visible = true;
				for (e in gameObjects){
					gameObjects[e].boundingVolume.visible = true;
				}
			}
			break;
		case 66:// B
		case 87:// b
			if (keyPressed != 0){
				keyPressed--;
			}
			else{
				keyPressed = 5;
				createBullet();
			}
			break;

		case 49:
			camera = cameraOrto;
			updateCamera();
			break;
		case 50:
			camera = cameraPerspStatic;
			updateCamera();
			break;
		case 51:
			camera = cameraPerspShip;
			updateCamera();
			break;
		case 65://A
		case 86://a
			for (e in materials){
				materials[e].wireframe = !materials[e].wireframe
			}
			break;
		case 37:  //seta esquerda
			if(nave.acelerationx == 0){
				nave.acelerationx = -acelerationx; //troca o sinal da aceleracao
			}
			else if(nave.acelerationx > 0){
				nave.acelerationx = -acelerationx2;
			}
			break;

		case 39:  //seta direita
			if(nave.acelerationx == 0){
				nave.acelerationx = acelerationx;
			}
			else if(nave.acelerationx < 0){
				nave.acelerationx = acelerationx2;
			}
			break;
		case 80: //P
			if(gameObjects[0].speedx == 0 && gameObjects[0].speedz == 0){
				for ( i=0;i<8;i++){
					var angle = Math.random() * (2*Math.PI);
					(gameObjects[i]).speedz =  alienSpeed*(Math.cos(angle));
					(gameObjects[i]).speedx =  alienSpeed*(Math.sin(angle));

				}
			}
			else if (gameObjects[0].speedx != 0 || gameObjects[0].speedz != 0){ //problema de quando chocaram contra a parede
				for ( i=0;i<8;i++){  // passar o 8 para numero de aliens
					(gameObjects[i]).speedz = 0;
					(gameObjects[i]).speedx = 0;
					(gameObjects[i]).acelerationz = 0;
					(gameObjects[i]).acelerationx = 0;

				}
			}
			break;

}}

//------------------------- Key Up ------------------------------------------
function onKeyUp(tecla){
	switch(tecla.keyCode){
		case 66:
		case 87:
			keyPressed = 0;
			break;
		case 37://seta esquerda
			nave.acelerationx = acelerationx2;
			break;
		case 39://seta direita
			nave.acelerationx = -acelerationx2;
			break;
	}
}

//---------------------------------------------------------------------------

function animation(){
	var e,i;
	timePassed = clock.getDelta();
	for (e in gameObjects){
		(gameObjects[e]).update(timePassed);
	}
	for(e=0; e<gameObjects.length;e++){
		for(i=e+1;i<gameObjects.length;i++){
			if (gameObjects[e].hasColision(gameObjects[i])){
				gameObjects[e].treatColision(gameObjects[i]);
				gameObjects[i].treatColision(gameObjects[e]);
			}
		}

		(gameObjects[e]).newPos();
		if(gameObjects[e].dead){
			scene.remove(gameObjects[e]);
			gameObjects.splice(e,1);
			e--;
		}
	}
	render();
	requestAnimationFrame(animation);
}
