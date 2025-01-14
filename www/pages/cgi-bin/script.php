#!/usr/bin/php
<?php
// En-têtes nécessaires pour une réponse CGI correcte
header("Content-Type: text/html");

// Afficher les informations de l'environnement CGI
echo "<html>";
echo "<head><title>CGI Test</title><link rel=\"stylesheet\" href=\"../../styles.css\"></head>";
echo "<body>";
echo "<h1>PHP CGI executed with success !</h1>";
echo "<p>Server has correctly handle .php file</p>";
echo "<a href=\"../../index.html\" id=\"return\">Go Back Home</a>";
echo "</body>";
echo "</html>";
?>