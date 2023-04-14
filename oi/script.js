// Configuração do Firebase
const firebaseConfig = {
    apiKey: "SUA_API_KEY",
    authDomain: "SEU_AUTH_DOMAIN",
    databaseURL: "SUA_DATABASE_URL",
    projectId: "SEU_PROJECT_ID",
    storageBucket: "SEU_STORAGE_BUCKET",
    messagingSenderId: "SEU_SENDER_ID",
    appId: "SEU_APP_ID"
  };
  firebase.initializeApp(firebaseConfig);
  const database = firebase.database();
  
  // Cria referência para o nó de temperatura no Firebase
  const temperatureRef = database.ref('temperature');
  
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
  