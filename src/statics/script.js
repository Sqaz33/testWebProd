document.addEventListener('DOMContentLoaded', function() {
    const messageButton = document.getElementById('messageButton');
    const messageParagraph = document.getElementById('message');

    messageButton.addEventListener('click', function() {
        messageParagraph.textContent = 'Hello! You clicked the button.';
    });
});
