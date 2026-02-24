const Quit = () => {
  console.log("Quit");
};

const Back = () => {
  window.location = "../MainMenu/MainMenu.html";
};

const NewDungeon = () => {
  window.location = "../NewDungeon/NewDungeon.html";
};

const LoadDungeon = () => {
  window.location = "../LoadDungeon/LoadDungeon.html";
};

const Play = (event) => {
  event.preventDefault();
  const name = document.querySelector("#name").value;
  if (!name) return;
  Send("Play", name);
};
