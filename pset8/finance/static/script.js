let password = document.getElementById("pass1")
let confirmation = document.getElementById("pass2")
let char_count = document.getElementById("char_count")
let lower_char = document.getElementById("lower_char")
let upper_char = document.getElementById("upper_char")
let num_char = document.getElementById("num_char")
let special_char = document.getElementById("special_char")
let submit_btn = document.getElementById("submit")

function password_check(e) {
    /** This function is being used to find out the values input by the user in both the password and confirm password text boxes.
    * The results are fed back to the user using alerts.
    * **/

    //check for lower case
    if (!password.value.match(/[a-z]/)) {
        lower_char.innerHTML = "x";
        // lower_char.classList.add("invalid-req");
        // if (password.value.length > 0)
        // {
        //     password.classList.add("is-invalid")
        // }
        // if (confirmation.value.length > 0) {
        //     confirmation.classList.add("is-invalid")
        // }
    }
    else {
        lower_char.innerHTML = "✓";
        // lower_char.classList.remove("valid_req");
        // if (password.value.length > 0) {
        //     password.classList.remove("is-valid")
        // }
        // if (confirmation.value.length > 0) {
        //     confirmation.classList.remove("is-valid")
        // }
    }

    //Validating length
    if (password.value.length < 8) {
        count_char.innerHTML = "x";
    }
    else {
        count_char.innerHTML = "✓";
    }

    //check for upper case
    if (!password.value.match(/[A-Z]/)) {
        upper_char.innerHTML = "x";
    }
    else {
        upper_char.innerHTML = "✓";
    }

    //check for number
    if (!password.value.match(/\d+/g)) {
        num_char.innerHTML = "x";
    }
    else {
        num_char.innerHTML = "✓";
    }

    //check for special char
    if (!password.value.match(/['!', '?', '#', '$']/)) {
        special_char.innerHTML = "x";
    }
    else {
        special_char.innerHTML = "✓";
    }

    //Validation confirmation matches
    if (pass1.value != pass2.value) {
        matched_pass.innerHTML = "x";
    }
    else {
        matched_pass.innerHTML = "✓";
    }

    if (password.value.match(/[a-z]/) && (password.value.length >= 8) && password.value.match(/[A-Z]/) && password.value.match(/\d+/g) && password.value.match(/['!', '?', '#', '$']/) && pass1.value === pass2.value) {
        console.log('ok')
        submit_btn.removeAttribute('disabled');
    }
    else {
        submit_btn.disabled = true;
    }
};