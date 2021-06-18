<?php

	require "config/config.php";

	$isUpdated = false;

	// Make sure that required fields are filled out (e.g. track name and genre)
	if ( !isset($_POST['dvd_title']) || empty($_POST['dvd_title']) ) {

		$error = "Please fill out all required fields.";
	}
	else {

		$mysqli = new mysqli(DB_HOST, DB_USER, DB_PASS, DB_NAME);
		if ( $mysqli->connect_errno ) {
			echo $mysqli->connect_error;
			exit();
		}

		// ---- Using prepared statements
		$statement = $mysqli->prepare("UPDATE dvd_titles 
		SET title = ?,  release_date = ?, award = ?, label_id = ?, sound_id = ?, 
			genre_id = ?, rating_id = ?, format_id = ? 
		WHERE dvd_title_id = ?");

		// Bind the user inputs with the SQL statement
		// First param: the specific data types allowed
		// Second param: the user inputs
		$statement->bind_param("sssiiiiii", $_POST["dvd_title"], $_POST["release_date"], $_POST["award"], $_POST["label"], 
		$_POST["sound"], $_POST["genre"], $_POST["rating"], $_POST["format"], $_POST["dvd_title_id"]);

		// execute the statement. execute() will return false if there is an error with execution
		$executed = $statement->execute();
		// check for errors
		if(!$executed) {
			echo $mysqli->error;
		}

		// Check that one record was affected
		// affected_rows returns how many records were changed. This should be ONE (works for prepared statements too)
		if($mysqli->affected_rows == 1) {
			$isUpdated = true;
		}
		else {
			echo "..." . $mysqli->affected_rows . "...." . $mysqli->error;
		}

		// close the statement when finished
		$statement->close();
		$mysqli->close();

	}
?>

<!DOCTYPE html>
<html>
<head>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1">
	<title>Edit Confirmation | Song Database</title>
	<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css">
</head>
<body>
	<ol class="breadcrumb">
		<li class="breadcrumb-item"><a href="index.php">Home</a></li>
		<li class="breadcrumb-item"><a href="search_form.php">Search</a></li>
		<li class="breadcrumb-item"><a href="search_results.php">Results</a></li>
		<li class="breadcrumb-item"><a href="details.php?dvd_title_id=<?php echo $_POST['dvd_title_id']; ?>">Details</a></li>
		<li class="breadcrumb-item"><a href="edit_form.php?dvd_title_id=<?php echo $_POST['dvd_title_id']; ?>">Edit</a></li>
		<li class="breadcrumb-item active">Confirmation</li>
	</ol>
	<div class="container">
		<div class="row">
			<h1 class="col-12 mt-4">Edit Confirmation</h1>
		</div> <!-- .row -->
	</div> <!-- .container -->
	<div class="container">
		<div class="row mt-4">
			<div class="col-12">

		<?php if ( isset($error) && !empty($error) ) : ?>
			<div class="text-danger">
				<?php echo $error; ?>
			</div>
		<?php endif; ?>

		<?php if ($isUpdated) : ?>
			<div class="text-success">
				<span class="font-italic"><?php echo $_POST['dvd_title']; ?></span> was successfully edited.
			</div>
		<?php endif; ?>

			</div> <!-- .col -->
		</div> <!-- .row -->
		<div class="row mt-4 mb-4">
			<div class="col-12">
				<a href="details.php?dvd_title_id=<?php echo $_POST['dvd_title_id']; ?>" role="button" class="btn btn-primary">Back to Details</a>
			</div> <!-- .col -->
		</div> <!-- .row -->
	</div> <!-- .container -->
</body>
</html>