var statusElement = document.getElementById('status');
var progressElement = document.getElementById('progress');
var spinnerElement = document.getElementById('spinner');

var Module = {
  preRun: [],
  postRun: [],
  print: (function () {
    var element = document.getElementById('output');
    if (element) element.value = ''; // clear browser cache
    return function (text) {
      if (arguments.length > 1) text = Array.prototype.slice.call(arguments).join(' ');
      // These replacements are necessary if you render to raw HTML
      //text = text.replace(/&/g, "&amp;");
      //text = text.replace(/</g, "&lt;");
      //text = text.replace(/>/g, "&gt;");
      //text = text.replace('\n', '<br>', 'g');
      console.log(text);
      if (element) {
        element.value += text + "\n";
        element.scrollTop = element.scrollHeight; // focus on bottom
      }
    };
  })(),
  canvas: (function () {
    var canvas = document.getElementById('canvas');

    // As a default initial behavior, pop up an alert when webgl context is lost. To make your
    // application robust, you may want to override this behavior before shipping!
    // See http://www.khronos.org/registry/webgl/specs/latest/1.0/#5.15.2
    ///canvas.addEventListener("webglcontextlost", function (e) { alert('WebGL context lost. You will need to reload the page.'); e.preventDefault(); }, false);

    return canvas;
  })(),
  setStatus: function (text) {
    if (!Module.setStatus.last) Module.setStatus.last = { time: Date.now(), text: '' };
    if (text === Module.setStatus.last.text) return;
    var m = text.match(/([^(]+)\((\d+(\.\d+)?)\/(\d+)\)/);
    var now = Date.now();
    if (m && now - Module.setStatus.last.time < 30) return; // if this is a progress update, skip it if too soon
    Module.setStatus.last.time = now;
    Module.setStatus.last.text = text;
  },
  totalDependencies: 0,
  monitorRunDependencies: function (left) {
    this.totalDependencies = Math.max(this.totalDependencies, left);
    Module.setStatus(left ? 'Preparing... (' + (this.totalDependencies - left) + '/' + this.totalDependencies + ')' : 'All downloads complete.');
  }
};
Module.setStatus('Downloading...');
window.onerror = function () {
  Module.setStatus('Exception thrown, see JavaScript console');
  // spinnerElement.style.display = 'none';
  Module.setStatus = function (text) {
    if (text) console.error('[post-exception status] ' + text);
  };
};

function makeMoveWASM(inputFEN) {
  // Allocate memory for the input string
  const inputPtr = Module._malloc(inputFEN.length + 1);
  Module.stringToUTF8(inputFEN, inputPtr, inputFEN.length + 1); 

  // Call the C function
  const resultPtr = Module.ccall(
    "make_random_valid_move", // name of C function
    "number", // return type (pointer to char)
    ["number"], // argument types (pointer to char)
    [inputPtr] // arguments (input string pointer)
  );

  // Get the result string from the result pointer
  const resultString = Module.UTF8ToString(resultPtr);

  // Free the memory used for the input and result strings
  Module._free(inputPtr);
  Module._free(resultPtr);

  console.log("Result string is: ", resultString);
  return resultString;
}