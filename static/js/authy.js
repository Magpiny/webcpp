
const err_mssg_elm = document.querySelector(".error-message");

err_mssg_elm.style.display = "block";

setTimeout(() => {
    err_mssg_elm.style.display = "none";
}, 5000);


//client-side form validation
document.querySelector('form').addEventListener('submit', function(e) {
    const email = document.getElementById('email').value;
    const emailRegex = /^[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*@(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?$/;

    if (!emailRegex.test(email)) {
        e.preventDefault();
        alert('Please enter a valid email address');
    }
});

// Function to format the date

function formatDate(date) {
    const options = {
        weekday: 'long',
        year: 'numeric',
        month: 'long',
        day: 'numeric'
    };
    return date.toLocaleDateString('en-US', options);
}

// Update the date in the footer
function updateDate() {
    const dateElement = document.getElementById('current-date');
    const currentDate = new Date();
    dateElement.textContent = `Today is ${formatDate(currentDate)}`;
}

// Initialize date on page load
document.addEventListener('DOMContentLoaded', () => {
    updateDate();
    // Update date every minute
    setInterval(updateDate, 60000);
});

document.querySelector('.register-form').addEventListener('submit', function(e) {
    e.preventDefault();
    const token = document.querySelector('input[name="_csrf"]').value;

    fetch('/register', {
        method: 'POST',
        headers: {
            'X-CSRF-Token': token,
            'Content-Type': 'application/x-www-form-urlencoded',
        },
        body: new URLSearchParams(new FormData(this))
    }).then(response => {
        if (response.redirected) {
            window.location.href = response.url;
        }
    });
});
