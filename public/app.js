document.getElementById("btn").addEventListener("click", () => {
  document.getElementById("out").textContent =
    "Button clicked at " + new Date().toLocaleTimeString();
});
