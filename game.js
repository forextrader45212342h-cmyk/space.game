// --- THREE.JS GLOBAL SETUP ---
const scene = new THREE.Scene();
const camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 5000);
const renderer = new THREE.WebGLRenderer({ antialias: true });
renderer.setSize(window.innerWidth, window.innerHeight);
document.body.appendChild(renderer.domElement);

// Lighting setup for realistic reflections
const ambientLight = new THREE.AmbientLight(0x444444);
scene.add(ambientLight);
const sunLight = new THREE.DirectionalLight(0xffffff, 1.2);
sunLight.position.set(500, 400, 500);
scene.add(sunLight);

// --- UNIVERSAL OBJECTS CREATION ---
// 1. Particle Starfield Galaxy Background
const starGeometry = new THREE.BufferGeometry();
const starCount = 2000;
const starPositions = new Float32Array(starCount * 3);
for(let i=0; i<starCount*3; i++) {
    starPositions[i] = (Math.random() - 0.5) * 4000;
}
starGeometry.setAttribute('position', new THREE.BufferAttribute(starPositions, 3));
const textureLoader = new THREE.TextureLoader();
const starMaterial = new THREE.PointsMaterial({ color: 0xffffff, size: 1.2 });
const starField = new THREE.Points(starGeometry, starMaterial);
scene.add(starField);

// 2. Earth Sphere Setup (Textures map automatically if file exists)
const earthGeo = new THREE.SphereGeometry(50, 64, 64);
const earthMat = new THREE.MeshStandardMaterial({ color: 0x1144aa, roughness: 0.6 });

textureLoader.load('earth.jpg', function(texture) {
    earthMat.map = texture;
    earthMat.needsUpdate = true;
});
const earth = new THREE.Mesh(earthGeo, earthMat);
earth.position.set(0, -50, 0);
scene.add(earth);

// 3. Mars Base & Rover Ecosystem
const marsGeo = new THREE.SphereGeometry(30, 64, 64);
const marsMat = new THREE.MeshStandardMaterial({ color: 0xcc4422 });
textureLoader.load('mars.jpg', function(texture) {
    marsMat.map = texture;
    marsMat.needsUpdate = true;
});
const mars = new THREE.Mesh(marsGeo, marsMat);
mars.position.set(0, 600, -300);
scene.add(mars);

// Mars Rover Indicator Mesh
const roverGeo = new THREE.BoxGeometry(2, 1, 2);
const roverMat = new THREE.MeshBasicMaterial({ color: 0xffcc00 });
const rover = new THREE.Mesh(roverGeo, roverMat);
rover.position.set(0, 31, 0);
mars.add(rover);

// 4. Black Hole Singularity & Swirling Nebula Shader Mapping
const bhGeo = new THREE.SphereGeometry(18, 32, 32);
const bhMat = new THREE.MeshBasicMaterial({ color: 0x000000 });
const blackHole = new THREE.Mesh(bhGeo, bhMat);
blackHole.position.set(400, 1000, -600);
scene.add(blackHole);

const nebGeo = new THREE.TorusGeometry(35, 10, 16, 100);
const nebMat = new THREE.MeshBasicMaterial({ color: 0x6600cc, wireframe: true });
const nebula = new THREE.Mesh(nebGeo, nebMat);
nebula.position.copy(blackHole.position);
scene.add(nebula);

// Alien Planet Exoplanet system
const alienGeo = new THREE.SphereGeometry(22, 32, 32);
const alienMat = new THREE.MeshStandardMaterial({ color: 0x00ffaa, wireframe: true });
const alienPlanet = new THREE.Mesh(alienGeo, alienMat);
alienPlanet.position.set(-700, 800, -900);
scene.add(alienPlanet);

// --- CONTROLLABLE PLAYER ROCKET VEHICLE ---
const rocketGeo = new THREE.CylinderGeometry(1, 2.5, 10, 12);
const rocketMat = new THREE.MeshStandardMaterial({ color: 0xdddddd, metalness: 0.5 });
const rocket = new THREE.Mesh(rocketGeo, rocketMat);
rocket.position.set(0, 5, 0);
scene.add(rocket);

// --- CORE SIMULATION STATE ---
let state = { altitude: 0, o2: 100, isDead: false };
let isTelescope = false;

camera.position.set(0, 20, 50);
camera.lookAt(rocket.position);

// --- PROCEDURAL AUDIO FLOW ---
const audioContext = new (window.AudioContext || window.webkitAudioContext)();
function triggerEngineSound() {
    if (audioContext.state === 'suspended') audioContext.resume();
    const osc = audioContext.createOscillator();
    const gain = audioContext.createGain();
    osc.type = 'sawtooth';
    osc.frequency.setValueAtTime(55, audioContext.currentTime); // Low engine bass
    gain.gain.setValueAtTime(0.25, audioContext.currentTime);
    osc.connect(gain);
    gain.connect(audioContext.destination);
    osc.start();
    osc.stop(audioContext.currentTime + 0.4);
}

// --- ENGINE RENDERING LOOP ---
function renderFrame() {
    requestAnimationFrame(renderFrame);

    // Continuous celestial motions
    earth.rotation.y += 0.001;
    mars.rotation.y += 0.003;
    nebula.rotation.z += 0.008;

    // Realtime system calculations
    state.altitude = Math.round(rocket.position.y * 10);
    document.getElementById('alt').innerText = state.altitude;

    if (state.altitude > 120) {
        document.getElementById('loc').innerText = "Deep Space Orbit";
        state.o2 -= 0.08; 
        document.getElementById('alert').innerText = "⚠️ SYSTEM ALERT: EXT-SPACE SUIT REQUIRED! O2 CRITICAL!";
    } else {
        document.getElementById('loc').innerText = "Earth Command Base";
        document.getElementById('alert').innerText = "";
        if (state.o2 < 100) state.o2 += 0.4;
    }

    // Black Hole Gravitational Pull Mapping
    let distanceToSingularity = rocket.position.distanceTo(blackHole.position);
    if (distanceToSingularity < 200) {
        document.getElementById('alert').innerText = "🚨 GRAVITY WARNING: EVENT HORIZON DETECTED!";
        rocket.position.lerp(blackHole.position, 0.015); // Attraction physics
        if (distanceToSingularity < 25) {
            handleRespawn("Singularity Crushed Event Horizon Intersection!");
        }
    }

    if (state.o2 <= 0) {
        handleRespawn("O2 Exhausted in Infinite Vacuum!");
    }

    document.getElementById('o2').innerText = Math.max(0, Math.round(state.o2));

    // Dynamic Camera Tracking
    camera.position.x = rocket.position.x;
    camera.position.y = rocket.position.y + 18;
    camera.position.z = rocket.position.z + 48;

    renderer.render(scene, camera);
}

function handleRespawn(logMessage) {
    alert("💀 RESPAWN STATUS: " + logMessage + "\\nReturning to Safe Sector Base (Zameen Home).");
    rocket.position.set(0, 5, 0);
    state.o2 = 100;
    document.getElementById('alert').innerText = "";
}

// --- BUTTON TRIGGERS REGISTER ---
document.getElementById('btn-up').addEventListener('click', () => {
    rocket.position.y += 6;
    rocket.position.z -= 1.5;
    triggerEngineSound();
});
document.getElementById('btn-left').addEventListener('click', () => { rocket.position.x -= 4; });
document.getElementById('btn-right').addEventListener('click', () => { rocket.position.x += 4; });
document.getElementById('btn-zoom').addEventListener('click', () => {
    isTelescope = !isTelescope;
    if (isTelescope) {
        camera.fov = 20; // 6x Telescope view
        document.getElementById('zoom').innerText = "6x (Telescope Track)";
    } else {
        camera.fov = 75;
        document.getElementById('zoom').innerText = "1x";
    }
    camera.updateProjectionMatrix();
});

renderFrame();

window.addEventListener('resize', () => {
    camera.aspect = window.innerWidth / window.innerHeight;
    camera.updateProjectionMatrix();
    renderer.setSize(window.innerWidth, window.innerHeight);
});
