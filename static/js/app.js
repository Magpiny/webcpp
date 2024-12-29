document.addEventListener('DOMContentLoaded', () => {
    const nav = document.querySelector('.nav-links');
    const header = document.querySelector('header');

    // Create and add toggle button
    const toggleBtn = document.createElement('button');
    toggleBtn.className = 'nav-toggle';
    toggleBtn.innerHTML = '☰';
    header.insertBefore(toggleBtn, nav);

    // Create overlay
    const overlay = document.createElement('div');
    overlay.className = 'nav-overlay';
    document.body.appendChild(overlay);

    // Toggle navigation function
    const toggleNav = (show) => {
        nav.classList.toggle('active', show);
        toggleBtn.classList.toggle('active', show);
        overlay.classList.toggle('active', show);
        toggleBtn.innerHTML = show ? '✕' : '☰';

        // Prevent body scroll when menu is open
        document.body.style.overflow = show ? 'hidden' : '';
    };

    // Toggle button click handler
    toggleBtn.addEventListener('click', (e) => {
        e.stopPropagation();
        const willShow = !nav.classList.contains('active');
        toggleNav(willShow);
    });

    // Close menu when clicking overlay
    overlay.addEventListener('click', () => {
        toggleNav(false);
    });

    // Handle escape key
    document.addEventListener('keydown', (e) => {
        if (e.key === 'Escape' && nav.classList.contains('active')) {
            toggleNav(false);
        }
    });

    // Close menu when clicking a link
    nav.querySelectorAll('a').forEach(link => {
        link.addEventListener('click', () => {
            toggleNav(false);
        });
    });

    // Handle window resize
    let resizeTimer;
    window.addEventListener('resize', () => {
        clearTimeout(resizeTimer);
        resizeTimer = setTimeout(() => {
            if (window.innerWidth > 768 && nav.classList.contains('active')) {
                toggleNav(false);
            }
        }, 250);
    });
});

//client-side form validation
document.querySelector('form').addEventListener('submit', function(e) {
    const email = document.getElementById('email').value;
    const emailRegex = /^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$/;

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
