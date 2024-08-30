function regClick() {
    const data = {
        login: document.getElementById("login").value,
        pword: document.getElementById("pword").value
    };
    fetch('/reg', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify(data)
    })
    .then(response => response.text())
    .then(result => {
        console.log('Ответ сервера:', result);
    })
    .catch(error => {
        console.error('Ошибка:', error);
    });
}


