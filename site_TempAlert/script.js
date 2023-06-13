// Configuração do Firebase
const firebaseConfig = {
    apiKey: "AIzaSyC-PuRZzA8cw1xcetgVP-V2_UhDhbOZaLA",
    authDomain: "ledwifi-a54cf.firebaseapp.com",
    databaseURL: "https://ledwifi-a54cf-default-rtdb.firebaseio.com",
    projectId: "ledwifi-a54cf",
    storageBucket: "ledwifi-a54cf.appspot.com",
    messagingSenderId: "593740331417",
    appId: "1:593740331417:web:b6dfa956b65d9d48d8fb08",
    measurementId: "G-73FWH71DYF"
  };
  firebase.initializeApp(firebaseConfig);
  const database = firebase.database();
  
  // Cria referência para o nó de temperatura no Firebase
  const temperatureRef = database.ref('tempInadequada');
  
  // Listener para atualizar a lista de alertas
  temperatureRef.on('value', (snapshot) => {
      const temperatureTable = document.getElementById('temperature-table');
      temperatureTable.innerHTML = `
          <tr>
              <th>Data</th>
              <th>Hora</th>
              <th>Temperatura (°C)</th>
          </tr>
      `;
      snapshot.forEach((childSnapshot) => {
          const temperature = childSnapshot.val().value;
          const date = childSnapshot.val().date;
          const time = childSnapshot.val().time;
          const row = `
              <tr>
                  <td>${date}</td>
                  <td>${time}</td>
                  <td>${temperature}</td>
              </tr>
          `;
          temperatureTable.innerHTML += row;
      });
  });
  