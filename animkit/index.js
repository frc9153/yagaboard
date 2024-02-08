const Colors = [
    [0, 0, 0], // Black
    [127, 127, 127], // Gr(a|e)y
    [255, 255, 255], // White
    [255, 0, 0], // Red
    [0, 255, 0], // Green
    [0, 0, 255], // Blue
    [255, 0, 255], // Magenta
    [127, 0, 255], // Purple
    [0, 255, 255], // Cyan
    [255, 255, 0], // Yellow
    [255, 127, 0], // Orange
];
if (Colors.length > 16) throw "Color amount exceeds 4 bits";

let currentColorIndex = 0;

// Make the color selection buttons
const colorContainer = $el("#color-container");
for (const [i, c] of Object.entries(Colors)) {
    const button = $e(
        "button",
        colorContainer,
        {
            classes: ["color-button"],
            "style.backgroundColor": `rgb(${c.join(",")})`
        }
    );

    button.addEventListener("click", function() {
        currentColorIndex = i;
    });
}

const GRID_WIDTH = 32;
const GRID_HEIGHT = 8 * 2;

const coordEl = $el("#coords");
const canvas = $el("#canvas");
const ctx = canvas.getContext("2d");

const scalar = 5;
canvas.width = GRID_WIDTH * scalar;
canvas.height = GRID_HEIGHT * scalar;

let pixels = new Array(GRID_WIDTH * GRID_HEIGHT).fill(0);
let mousePosPx = [0, 0];
let gridPos = [0, 0];
let mouseDown = false;

function getIndex(x, y) {
    return (y * GRID_WIDTH) + x;
}

function render() {
    // We're gonna paint over everything so we really don't need this anymore!
    // ctx.clearRect(0, 0, canvas.width, canvas.height);
    
    // Don't wrap around if user does some weird click dragging
    if (
        !(mousePosPx[0] < 0 || mousePosPx[0] > canvas.width)
        && !(mousePosPx[1] < 0 || mousePosPx[1] > canvas.height)
    ) {
        if (mouseDown) pixels[getIndex(...gridPos)] = currentColorIndex;
    }

    // Very ineffecient but its like 512 pixels so who really cares (time
    // complexity nerds shaking and crying)
    for (let y = 0; y < GRID_HEIGHT; y++) {
        for (let x = 0; x < GRID_WIDTH; x++) {
            let color = Colors[pixels[getIndex(x, y)]];
            ctx.fillStyle = `rgb(${color.join(",")})`
            ctx.fillRect(x * scalar, y * scalar, scalar, scalar);
        }
    }
    ctx.fillStyle = `rgba(${Colors[currentColorIndex].join(",")}, 0.5)`
    ctx.fillRect(gridPos[0] * scalar, gridPos[1] * scalar, scalar, scalar);
}

window.addEventListener("mousemove", function(event) {
    const rect = canvas.getBoundingClientRect();

    mousePosPx = [
        Math.floor(((event.clientX - rect.left) / rect.width) * canvas.width),
        Math.floor(((event.clientY - rect.top) / rect.height) * canvas.height),
    ];

    gridPos = [
        Math.floor(mousePosPx[0] / scalar),
        Math.floor(mousePosPx[1] / scalar)
    ];
    coordEl.innerText = `(${gridPos[0]}, ${gridPos[1]})`;

    render();
});

canvas.addEventListener("mousedown", function() { mouseDown = true; });
window.addEventListener("mouseup", function() { mouseDown = false; });

canvas.addEventListener("click", function() {
    pixels[getIndex(...gridPos)] = currentColorIndex;
    render();
});

function getSmallestLengthSizeBits(pixels) {
    let best = 0;
    let current = 0;
    let val = -1;

    for (const p of pixels) {
        if (p !== val) {
            current = 0;
            val = p;
            continue;
        }

        current++;

        if (current > best) best = current;
    }

    return Math.ceil(Math.log2(best + 1));
}

function optimize(pixels) {
    const COLOR_SIZE = 4;
    let repeatSize = getSmallestLengthSizeBits(pixels);
    
    console.info(`We think the best repeat size is ${repeatSize}`);

    let repeatCount = 0;
    let lastColor = -1;
    let bits = "";

    // "Header"
    bits += (repeatSize >>> 0).toString(2).padStart(repeatSize, "0");

    function commit() {
        if (repeatCount >= Math.pow(2, repeatSize)) throw ":(";
        if (lastColor >= Math.pow(2, COLOR_SIZE)) throw ":(";

        bits += (repeatCount >>> 0).toString(2).padStart(repeatSize, "0");
        bits += (lastColor >>> 0).toString(2).padStart(COLOR_SIZE, "0");
        repeatCount = 0;
    }

    for (const colorIndex of pixels) {
        if (repeatCount + 1 >> repeatSize) {
            commit();
        }

        if (colorIndex === lastColor) {
            repeatCount++;
            continue;
        }

        commit();
        lastColor = colorIndex;
    }

    console.log(bits);

    let out = [];
    let buf = "";

    for (const bit of bits) {
        buf += bit;

        if (buf.length === 8) {
            out.push(parseInt(buf, 2));
            buf = "";
        }
    }
    if (buf.length) out.push(parseInt(buf, 2));

    return out;
}

$el("#export").addEventListener("click", function() {
    const data = optimize(pixels);
    let code = "";
    let line = "";
    for (const byte of data) {
        line += `0x${byte.toString(16).padStart(2, "0")}, `;
        if (line.length <= 80) continue;

        code += "\n" + line.trim();
        line = "";
    }
    code += "\n" + line;
    code = code.trim();

    $el("#exported-code").innerText = code;
});

$el("#wipe").addEventListener("click", function() {
    pixels.fill(0);
    render();
});
