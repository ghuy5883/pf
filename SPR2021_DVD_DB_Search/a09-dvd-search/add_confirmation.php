<?php
	require "config/config.php";

	if ( !isset($_POST['dvd_name']) || empty($_POST['dvd_name']) ) {

		// Missing required fields.
		$error = "Please fill out all required fields.";

	} else {
		// All required fields provided.
		$mysqli = new mysqli(DB_HOST, DB_USER, DB_PASS, DB_NAME);
		if ( $mysqli->errno ) {
			echo $mysqli->error;
			exit();
		}

		if ( isset($_POST['dvd_name']) && !empty($_POST['dvd_name']) ) {
			// User selected dvd_name value.
			$dvd_name = $_POST['dvd_name'];
		} else {
			// User did not select dvd_name value.
			$dvd_name = "null";
		}

		if ( isset($_POST['rating']) && !empty($_POST['rating']) ) {
			// User selected rating value.
			$rating = $_POST['rating'];
		} else {
			// User did not select rating value.
			$rating = NULL;
		}

		if ( isset($_POST['genre']) && !empty($_POST['genre']) ) {
			// User selected rating value.
			$genre = $_POST['genre'];
		} else {
			// User did not select rating value.
			$genre = NULL;
		}

		if ( isset($_POST['label']) && !empty($_POST['label']) ) {
			// User selected label value.
			$label = $_POST['label'];
		} else {
			// User did not select label value.
			$label = NULL;
		}

		if ( isset($_POST['format']) && !empty($_POST['format']) ) {
			// User typed in format field.
			$format = $_POST['format'];
		} else {
			// User did not type in format field.
			$format = NULL;
		}
		
		if ( isset($_POST['sound']) && !empty($_POST['sound']) ) {
			// User typed in sound field.
			$sound = $_POST['sound'];
		} else {
			// User did not type in sound field.
			$sound = NULL;
		}

		if ( isset($_POST['award']) && !empty($_POST['award']) ) {
			// User typed in sound field.
			$award = $_POST['award'];
		} else {
			// User did not type in sound field.
			$award = NULL;
		}	
		
		if ( isset($_POST['release_date']) && !empty($_POST['release_date']) ) {
			// User typed in sound field.
			$release_date = $_POST['release_date'];
		} else {
			// User did not type in sound field.
			$release_date = NULL;
		}
		
		/*
		$sql = "INSERT INTO dvd_titles (title, release_date, award, label_id, sound_id, genre_id, rating_id, format)
						VALUES ('" 
						. $dvd_name
						. "', "
						. $release_date
						.", "
						. $award
						.", "
						. $label
						.", "
						. $sound
						.", "
						. $genre
						.", "
						. $rating
						.", "
						.$format
						.");";

		// echo "<hr>" . $sql . "<hr>";

		$results = $mysqli->query($sql);
		if ( !$results ) {
			echo $mysqli->error;
			exit();
		}
		*/

		$statement = $mysqli->prepare("INSERT INTO dvd_titles(title, release_date, award, label_id, sound_id, genre_id, rating_id, format_id)
										VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
		$statement->bind_param("sssiiiii", $dvd_name, $release_date, $award, $label, $sound, $genre, $rating, $format);
		$executed = $statement->execute();
		if(!$executed) {
			echo $mysqli->error;
		}
		else {
			$isUpdated = true;
		}
		// close the statement when finished
		$statement->close();
		// Check affected record.

		$mysqli->close();
	}
?>

<!DOCTYPE html>
<html>
<head>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1">
	<title>Add Confirmation | Song Database</title>
	<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css">
</head>
<body>
	<ol class="breadcrumb">
		<li class="breadcrumb-item"><a href="index.php">Home</a></li>
		<li class="breadcrumb-item"><a href="add_form.php">Add</a></li>
		<li class="breadcrumb-item active">Confirmation</li>
	</ol>
	<div class="container">
		<div class="row">
			<h1 class="col-12 mt-4">Add a Song</h1>
		</div> <!-- .row -->
	</div> <!-- .container -->
	<div class="container">
		<div class="row mt-4">
			<div class="col-12">

	<?php if ( isset($error) && !empty($error) ) : ?>

		<div class="text-danger">
			<?php echo $error; ?>
		</div>

	<?php else : ?>

		<?php if ($isUpdated) : ?>
			<div class="text-success">
				<span class="font-italic"><?php echo $dvd_name; ?></span> was successfully added.
			</div>
		<?php else : ?>
			<div class="text-warning">
				<span class="font-italic"><?php echo $dvd_name; ?></span> encountered an error while adding.
			</div>
		<?php endif; ?>

	<?php endif; ?>

			</div> <!-- .col -->
		</div> <!-- .row -->
		<div class="row mt-4 mb-4">
			<div class="col-12">
				<a href="add_form.php" role="button" class="btn btn-primary">Back to Add Form</a>
			</div> <!-- .col -->
		</div> <!-- .row -->
	</div> <!-- .container -->
</body>
</html>