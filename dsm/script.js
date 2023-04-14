// Inicializar o Firebase
const config = {
    apiKey: 'sua-api-key',
    authDomain: 'seu-dominio.firebaseapp.com',
    databaseURL: 'https://seu-dominio.firebaseio.com',
    projectId: 'seu-projeto-id',
    storageBucket: 'seu-bucket.appspot.com',
    messagingSenderId: 'seu-id-de-mensagem'
  };
  firebase.initializeApp(config);
  
  // Obter a tabela
  const table = document.querySelector('#temperatures tbody');
  
  // Atualizar as temperaturas quando os dados do Firebase mudarem
  firebase.database().ref('temperatures').on('value', snapshot => {
    const data = snapshot.val();
  
    // Limpar a tabela
    table.innerHTML = '';
  
    // Adicionar cada temperatura à tabela
    Object.keys(data).forEach(key => {
      const temp = data[key];
  
      // Criar uma nova linha na tabela
      const row = table.insertRow();
  
      // Adicionar as células com os dados da temperatura
      const dateCell = row.insertCell();
      dateCell.textContent = temp.date;
  
      const timeCell = row.insertCell();
      timeCell.textContent = temp.time;
  
      const minutesCell = row.insertCell();
      minutesCell.textContent = temp.minutes;
  
      const secondsCell = row.insertCell();
      secondsCell.textContent = temp.seconds;
  
      const tempCell = row.insertCell();
      tempCell.textContent = temp.temp;
  
      // Marcar as temperaturas com alerta
      if (temp.temp > 8) {
        tempCell.classList.add('warning');
      } else if (temp.temp < 0) {
        tempCell.classList.add('warning');
      }
    });
  });
  