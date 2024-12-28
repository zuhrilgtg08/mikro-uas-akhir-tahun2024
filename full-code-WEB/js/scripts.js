const mainURL = "http://192.168.39.43";

function getKontrolIrigasi() {
  fetch(mainURL + "/irigasi")
    .then((res) => res.text())
    .then((result) => {
      if (result == "ON") {
        btnWater.style.backgroundColor = "#f76c6c";
        btnWater.textContent = "Turn Off";
        imgWater.src = "../img/water-on.png";
      } else {
        btnWater.style.backgroundColor = "#5cb338";
        btnWater.textContent = "Turn On";
        imgWater.src = "../img/water-off.png";
      }
    })
    .catch((error) =>
      console.error("Error fetching control irrigation: ", error)
    );

}

function getKontrolPompa() {
  fetch(mainURL + "/relayPompa")
    .then((res) => res.text())
    .then((result) => {
      if (result == "ON") {
        btnPump.style.backgroundColor = "#f76c6c";
        btnPump.textContent = "Turn Off";
        imgPump.src = "../img/pump-on.png";
      } else {
        btnPump.style.backgroundColor = "#5cb338";
        btnPump.textContent = "Turn On";
        imgPump.src = "../img/pump-off.png";
      }
    })
    .catch((err) => {
     console.log('Error Fetch Pompa: ', err);
   })
}

function getLampuRuangan() {
  fetch(mainURL + "/kontrolLed")
    .then((response) => response.text())
    .then((data) => {
      if (data == "ON") {
        btnLamp.style.backgroundColor = "#f76c6c";
        btnLamp.textContent = "Turn Off";
        imgLamp.src = "../img/led-on.png";
      } else {
        btnLamp.style.backgroundColor = "#5cb338";
        btnLamp.textContent = "Turn On";
        imgLamp.src = "../img/led-off.png";
      }
    })
    .catch((err) => {
      console.log("Error Fetch LED: ", err);
    });
}

function getKontrolVentilasi() {
  fetch(mainURL + "/ventilasi")
    .then((res) => res.text())
    .then((result) => {
      if (result == "ON") {
        btnVent.style.backgroundColor = "#f76c6c";
        btnVent.textContent = "Turn Off";
        imgVent.src = "../img/vent-on.png";
      } else {
        btnVent.style.backgroundColor = "#5cb338";
        btnVent.textContent = "Turn On";
        imgVent.src = "../img/vent-off.png";
      }
    })
    .catch((err) => {
      console.log("Error Fetch VENT: ", err);
    });
}

function setKontrolPompa() {
  fetch(mainURL + "/relayPompa", {
    method: "POST",
  })
    .then((res) => res.text())
    .then(() => getKontrolPompa())
    .catch((err) => {
      console.log('Error Setting Kontrol Pompa: ', err);
    });
}

function setKontrolVentilasi() {
  fetch(mainURL + "/ventilasi", {
    method: "POST",
  })
    .then((res) => res.text())
    .then(() => getKontrolVentilasi())
    .catch((err) => {
      console.log("Error SET Vent: ", err);
    });
}

function setLampuRuangan() {
  fetch(mainURL + "/kontrolLed", {
    method: "POST",
  })
    .then((response) => response.text())
    .then(() => getLampuRuangan())
    .catch((err) => {
      console.log("Error SET LED: ", err);
    });
}

function setKontrolIrigasi() {
  fetch(mainURL + "/irigasi", {
    method: "POST",
  })
    .then((response) => response.text())
    .then(() => getKontrolIrigasi())
    .catch((error) => {
      console.log('Error setting irigation', error);
    });
}

document.addEventListener("DOMContentLoaded", function () {
  const navbarToggle = document.getElementById("navbarToggle");
  const navbarMenu = document.getElementById("navbarMenu");

  navbarToggle.addEventListener("click", function () {
    navbarMenu.classList.toggle("active");
  });
});

document.addEventListener("DOMContentLoaded", function () {
  getKontrolIrigasi();
  getLampuRuangan();
  getKontrolVentilasi();
  getKontrolPompa();
});

