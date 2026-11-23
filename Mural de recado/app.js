import { initializeApp } from "https://www.gstatic.com/firebasejs/10.8.0/firebase-app.js";
import { 
  getAuth, createUserWithEmailAndPassword, signInWithEmailAndPassword, signOut, onAuthStateChanged 
} from "https://www.gstatic.com/firebasejs/10.8.0/firebase-auth.js";
import { 
  getFirestore, collection, addDoc, onSnapshot, doc, deleteDoc 
} from "https://www.gstatic.com/firebasejs/10.8.0/firebase-firestore.js";


const firebaseConfig = {
  apiKey: "SUA_API_KEY",
  authDomain: "SEU_PROJETO.firebaseapp.com",
  projectId: "SEU_PROJETO",
  storageBucket: "SEU_PROJETO.appspot.com",
  messagingSenderId: "SEU_SENDER_ID",
  appId: "SEU_APP_ID"
};

const app = initializeApp(firebaseConfig);
const auth = getAuth(app);
const db = getFirestore(app);


const authContainer = document.getElementById('auth-container');
const appContainer = document.getElementById('app-container');
const userEmailSpan = document.getElementById('user-email');
const emailInput = document.getElementById('email');
const passwordInput = document.getElementById('password');


onAuthStateChanged(auth, (user) => {
  if (user) {
    authContainer.style.display = 'none';
    appContainer.style.display = 'block';
    userEmailSpan.textContent = user.email;
    carregarRecados(); 
  } else {
    authContainer.style.display = 'block';
    appContainer.style.display = 'none';
  }
});


document.getElementById('register-button').addEventListener('click', async () => {
  try {
    await createUserWithEmailAndPassword(auth, emailInput.value, passwordInput.value);
    alert('Conta registada com sucesso!');
  } catch (error) {
    alert('Erro ao registar a conta: ' + error.message);
  }
});


document.getElementById('login-button').addEventListener('click', async () => {
  try {
    await signInWithEmailAndPassword(auth, emailInput.value, passwordInput.value);
  } catch (error) {
    alert('Erro ao fazer login: ' + error.message);
  }
});


document.getElementById('logout-button').addEventListener('click', () => {
  signOut(auth);
});


document.getElementById('save').addEventListener('click', async () => {
  const titulo = document.getElementById('recado-titulo').value;
  const texto = document.getElementById('recado-texto').value;

  if (!titulo || !texto) return alert('Preencha todos os campos!');

  try {
    await addDoc(collection(db, "recados"), {
      titulo: titulo,
      texto: texto,
      autor: auth.currentUser.email,
      uid: auth.currentUser.uid,
      data: new Date().toLocaleString('pt-BR')
    });

    document.getElementById('recado-titulo').value = '';
    document.getElementById('recado-texto').value = '';
  } catch (error) {
    alert('Erro ao publicar recado: ' + error.message);
  }
});


function carregarRecados() {
  const muralList = document.getElementById('mural-list');

  onSnapshot(collection(db, "recados"), (snapshot) => {
    muralList.innerHTML = '';
    snapshot.forEach((docSnap) => {
      const recado = docSnap.data();
      const id = docSnap.id;

      const card = document.createElement('div');
      card.className = 'card-recado';
      card.innerHTML = `
        <h4>${recado.titulo}</h4>
        <p>${recado.texto}</p>
        <small>Publicado por: ${recado.autor} em ${recado.data}</small>
        ${recado.uid === auth.currentUser.uid ? `<button onclick="deletarRecado('${id}')">Excluir</button>` : ''}
      `;
      muralList.appendChild(card);
    });
  });
}

window.deletarRecado = async (id) => {
  if (confirm("Deseja apagar este recado?")) {
    try {
      await deleteDoc(doc(db, "recados", id));
    } catch (error) {
      alert("Erro ao excluir: " + error.message);
    }
  }
};