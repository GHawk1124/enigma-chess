document.getElementById("mybutton").addEventListener("click", () => {
  const inputString = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

  // Allocate memory for the input string
  const inputPtr = Module._malloc(inputString.length + 1);
  Module.stringToUTF8(inputString, inputPtr, inputString.length + 1);

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

  console.log(resultString);
});