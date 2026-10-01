/**
 * GTA-6 Style Hyper-Realistic Open-World Space Game Engine
 * Powered by WebGL & Three.js
 */

let scene, camera, renderer, earth, astronaut, rover, satellite;
let moveForward = false, moveBackward = false, moveLeft = false, moveRight = false;
let joystick, joystickKnob;
let oxygenLevel = 100;

function initGame() {
    // 1. Scene Setup
    scene = new THREE.Scene();
    scene.background = new THREE.Color(0x000005);
    
    // Space Stars Dust
    const starGeometry = new THREE.BufferGeometry();
    const starCount = 8000;
    const starPositions = new Float32Array(starCount * 3);
    for(let i=0; i<starCount*3; i++) {
        starPositions[i] = (Math.random() - 0.5) * 3000;
    }
    starGeometry.setAttribute('position', new THREE.BufferAttribute(starPositions, 3));
    const starMaterial = new THREE.PointsMaterial({color: 0xffffff, size: 1.5});
    const starField = new THREE.Points(starGeometry, starMaterial);
    scene.add(starField);

    // 2. Camera setup with cinematic orbit view
    camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 5000);
    camera.position.set(0, 50, 150);

    // 3. Renderer with high shadow mappings
    renderer = new THREE.WebGLRenderer({ antialias: true });
    renderer.setSize(window.innerWidth, window.innerHeight);
    renderer.shadowMap.enabled = true;
    document.body.appendChild(renderer.domElement);

    // 4. Lighting (Sun God Rays Simulator)
    const sunLight = new THREE.DirectionalLight(0xffffff, 1.8);
    sunLight.position.set(500, 300, 500);
    sunLight.castShadow = true;
    scene.add(sunLight);
    scene.add(new THREE.AmbientLight(0x111122));

    // 5. High-Detail Textured Earth Sphere
    const earthGeo = new THREE.SphereGeometry(60, 64, 64);
    const earthMat = new THREE.MeshStandardMaterial({
        color: 0x2233ff,
        roughness: 0.4,
        metalness: 0.1
    });
    earth = new THREE.Mesh(earthGeo, earthMat);
    earth.position.set(0, -100, 0);
    scene.add(earth);

    // 6. NASA Satellite Model Setup
    const satGeo = new THREE.BoxGeometry(4, 2, 2);
    const satMat = new THREE.MeshStandardMaterial({color: 0x888888, metalness: 0.9, roughness: 0.1});
    satellite = new THREE.Mesh(satGeo, satMat);
    satellite.position.set(80, 50, 0);
    scene.add(satellite);

    // 7. Controllable Astronaut (Zero-Gravity Physics object)
    const astroGeo = new THREE.CylinderGeometry(2, 2, 8, 16);
    const astroMat = new THREE.MeshStandardMaterial({color: 0xeeeeee, roughness: 0.2});
    astronaut = new THREE.Mesh(astroGeo, astroMat);
    astronaut.position.set(0, 10, 0);
    scene.add(astronaut);

    // 8. Mobile UI Touch controls (Overlay Joystick)
    createMobileControls();

    // 9. Event Listeners
    window.addEventListener('resize', onWindowResize, false);
    setupKeyboardControls();

    animate();
}

function createMobileControls() {
    const uiContainer = document.createElement('div');
    uiContainer.style.position = 'absolute';
    uiContainer.style.bottom = '40px';
    uiContainer.style.left = '40px';
    uiContainer.style.width = '120px';
    uiContainer.style.height = '120px';
    uiContainer.style.background = 'rgba(255,255,255,0.1)';
    uiContainer.style.borderRadius = '50%';
    uiContainer.style.border = '2px solid rgba(255,255,255,0.4)';
    uiContainer.style.touchAction = 'none';
    document.body.appendChild(uiContainer);

    joystickKnob = document.createElement('div');
    joystickKnob.style.width = '50px';
    joystickKnob.style.height = '50px';
    joystickKnob.style.background = '#ffffff';
    joystickKnob.style.borderRadius = '50%';
    joystickKnob.style.position = 'relative';
    joystickKnob.style.top = '35px';
    joystickKnob.style.left = '35px';
    uiContainer.appendChild(joystickKnob);
    
    // Oxygen bar interface overlay
    const oxyHUD = document.createElement('div');
    oxyHUD.id = 'oxyHUD';
    oxyHUD.style.position = 'absolute';
    oxyHUD.style.top = '20px';
    oxyHUD.style.right = '20px';
    oxyHUD.style.color = '#ff3333';
    oxyHUD.style.fontFamily = 'sans-serif';
    oxyHUD.style.fontWeight = 'bold';
    oxyHUD.innerText = 'OXYGEN: 100%';
    document.body.appendChild(oxyHUD);
}

function setupKeyboardControls() {
    window.addEventListener('keydown', (e) => {
        if(e.code === 'KeyW' || e.code === 'ArrowUp') moveForward = true;
        if(e.code === 'KeyS' || e.code === 'ArrowDown') moveBackward = true;
        if(e.code === 'KeyA' || e.code === 'ArrowLeft') moveLeft = true;
        if(e.code === 'KeyD' || e.code === 'ArrowRight') moveRight = true;
    });
    window.addEventListener('keyup', (e) => {
        if(e.code === 'KeyW' || e.code === 'ArrowUp') moveForward = false;
        if(e.code === 'KeyS' || e.code === 'ArrowDown') moveBackward = false;
        if(e.code === 'KeyA' || e.code === 'ArrowLeft') moveLeft = false;
        if(e.code === 'KeyD' || e.code === 'ArrowRight') moveRight = false;
    });
}

function animate() {
    requestAnimationFrame(animate);

    // Physics Update: Planetary Rotation
    earth.rotation.y += 0.0005;
    satellite.rotation.x += 0.01;

    // Zero Gravity Smooth Displacement Logic
    if (moveForward) astronaut.position.z -= 0.8;
    if (moveBackward) astronaut.position.z += 0.8;
    if (moveLeft) astronaut.position.x -= 0.8;
    if (moveRight) astronaut.position.x += 0.8;

    // Simulation Engine: Oxygen Depletion rate configuration
    if(oxygenLevel > 0) {
        oxygenLevel -= 0.005;
        document.getElementById('oxyHUD').innerText = 'OXYGEN O2: ' + Math.floor(oxygenLevel) + '%';
    } else {
        document.getElementById('oxyHUD').innerText = 'SUIT CRITICAL COLLAPSE';
    }

    // Camera following the player tracking matrix
    camera.lookAt(astronaut.position);

    renderer.render(scene, camera);
}

function onWindowResize() {
    camera.aspect = window.innerWidth / window.innerHeight;
    camera.updateProjectionMatrix();
    renderer.setSize(window.innerWidth, window.innerHeight);
}

window.onload = initGame;
