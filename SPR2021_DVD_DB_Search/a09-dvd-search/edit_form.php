<?php
require "config/config.php";

// Make sure this page was passed a track id.
if(!isset($_GET["dvd_title_id"]) || empty($_GET["dvd_title_id"])) {
	echo "Invalid DVD ID.";
	exit();
}

// DB Connection.
$mysqli = new mysqli(DB_HOST, DB_USER, DB_PASS, DB_NAME);
if ( $mysqli->connect_errno ) {
	echo $mysqli->connect_error;
	exit();
}

$mysqli->set_charset('utf8');

// -- Get details of this track
$sql_dvd = "SELECT * FROM dvd_titles
	WHERE dvd_title_id = " . $_GET["dvd_title_id"] . ";";

$results_dvd = $mysqli->query($sql_dvd);
if(!$results_dvd) {
	echo $mysqli->error;
	quit();
}

$row_dvd = $results_dvd->fetch_assoc();

// Create SQL query.
$sql_genres = "SELECT * FROM genres";
$sql_ratings = "SELECT * FROM ratings";
$sql_labels = "SELECT * FROM labels";
$sql_formats = "SELECT * FROM formats";
$sql_sounds = "SELECT * FROM sounds";

$results_genres = $mysqli->query($sql_genres);
$results_ratings = $mysqli->query($sql_ratings);
$results_labels = $mysqli->query($sql_labels);
$results_formats = $mysqli->query($sql_formats);
$results_sounds = $mysqli->query($sql_sounds);
if( !$results_genres || !$results_ratings || !$results_labels
|| !$results_formats || !$results_sounds) {
	// display the error message
	echo $mysqli->error;
	// Termintate the program. No subsequent code runs.
	exit();
}

// Close DB Connection
$mysqli->close();

?>
<!DOCTYPE html>
<html>
<head>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1">
	<title>Edit Form | Song Database</title>
	<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css">
	<style>
		.form-check-label {
			padding-top: calc(.5rem - 1px * 2);
			padding-bottom: calc(.5rem - 1px * 2);
			margin-bottom: 0;
		}
	</style>
</head>
<body>
	<ol class="breadcrumb">
		<li class="breadcrumb-item"><a href="index.php">Home</a></li>
		<li class="breadcrumb-item"><a href="search_form.php">Search</a></li>
		<li class="breadcrumb-item"><a href="search_results.php">Results</a></li>
		<li class="breadcrumb-item"><a href="details.php?dvd_title_id=<?php echo $_GET["dvd_title_id"]; ?>">Details</a></li>
		<li class="breadcrumb-item active">Edit</li>
	</ol>
	<div class="container">
		<div class="row">
			<h1 class="col-12 mt-4 mb-4">Edit a Song</h1>
		</div> <!-- .row -->
	</div> <!-- .container -->
	<div class="container">

		<form action="edit_confirmation.php" method="POST">

			<div class="form-group row">
				<label for="title-id" class="col-sm-3 col-form-label text-sm-right">
					DVD Title: <span class="text-danger">*</span>
				</label>
				<div class="col-sm-9">
					<input type="text" class="form-control" id="title-id" name="dvd_title" value="<?php echo $row_dvd['title'] ?>"> 
				</div>
			</div> <!-- .form-group -->

			<div class="form-group row">
				<label for="genre-id" class="col-sm-3 col-form-label text-sm-right">
					Genre: 
				</label>
				<div class="col-sm-9">
					<select name="genre" id="genre-id" class="form-control">
						<option value="" disabled>-- Select One --</option>

						<?php while( $row = $results_genres->fetch_assoc() ): ?>
							
							<?php if($row["genre_id"] == $row_dvd["genre_id"]) :?>
								<option selected value="<?php echo $row['genre_id']; ?>">
									<?php echo $row['genre']; ?>
								</option>
							<?php else :?>
								<option value="<?php echo $row['genre_id']; ?>">
									<?php echo $row['genre']; ?>
								</option>
							<?php endif; ?>
							

						<?php endwhile; ?>
					</select>
				</div>
			</div> <!-- .form-group -->

			<div class="form-group row">
				<label for="rating-id" class="col-sm-3 col-form-label text-sm-right">
					Rating:
				</label>
				<div class="col-sm-9">
				<select name="rating" id="rating-id" class="form-control">
						<option value="" disabled>-- Select One --</option>

						<?php while( $row = $results_ratings->fetch_assoc() ): ?>
							
							<?php if($row["rating_id"] == $row_dvd["rating_id"]) :?>
								<option selected value="<?php echo $row['rating_id']; ?>">
									<?php echo $row['rating']; ?>
								</option>
							<?php else :?>
								<option value="<?php echo $row['rating_id']; ?>">
									<?php echo $row['rating']; ?>
								</option>
							<?php endif; ?>
							

						<?php endwhile; ?>
					</select>
				</div>
			</div> <!-- .form-group -->

			<div class="form-group row">
				<label for="rating-id" class="col-sm-3 col-form-label text-sm-right">
					Label:
				</label>
				<div class="col-sm-9">
				<select name="label" id="label-id" class="form-control">
						<option value="" disabled>-- Select One --</option>

						<?php while( $row = $results_labels->fetch_assoc() ): ?>
							
							<?php if($row["label_id"] == $row_dvd["label_id"]) :?>
								<option selected value="<?php echo $row['label_id']; ?>">
									<?php echo $row['label']; ?>
								</option>
							<?php else :?>
								<option value="<?php echo $row['label_id']; ?>">
									<?php echo $row['label']; ?>
								</option>
							<?php endif; ?>
							

						<?php endwhile; ?>
					</select>
				</div>
			</div> <!-- .form-group -->

			<div class="form-group row">
				<label for="rating-id" class="col-sm-3 col-form-label text-sm-right">
					Format:
				</label>
				<div class="col-sm-9">
				<select name="format" id="format-id" class="form-control">
						<option value="" disabled>-- Select One --</option>

						<?php while( $row = $results_formats->fetch_assoc() ): ?>
							
							<?php if($row["format_id"] == $row_dvd["format_id"]) :?>
								<option selected value="<?php echo $row['format_id']; ?>">
									<?php echo $row['format']; ?>
								</option>
							<?php else :?>
								<option value="<?php echo $row['format_id']; ?>">
									<?php echo $row['format']; ?>
								</option>
							<?php endif; ?>
							

						<?php endwhile; ?>
					</select>
				</div>
			</div> <!-- .form-group -->

			<div class="form-group row">
				<label for="rating-id" class="col-sm-3 col-form-label text-sm-right">
					Sound:
				</label>
				<div class="col-sm-9">
				<select name="sound" id="sound-id" class="form-control">
						<option value="" disabled>-- Select One --</option>

						<?php while( $row = $results_sounds->fetch_assoc() ): ?>
							
							<?php if($row["sound_id"] == $row_dvd["sound_id"]) :?>
								<option selected value="<?php echo $row['sound_id']; ?>">
									<?php echo $row['sound']; ?>
								</option>
							<?php else :?>
								<option value="<?php echo $row['sound_id']; ?>">
									<?php echo $row['sound']; ?>
								</option>
							<?php endif; ?>
							

						<?php endwhile; ?>
					</select>
				</div>
			</div> <!-- .form-group -->

			<div class="form-group row">
				<label for="rating-id" class="col-sm-3 col-form-label text-sm-right">
					Award:
				</label>
				<div class="col-sm-9">
					<input type="text" class="form-control" id="title-id" name="award" value="<?php echo $row_dvd['award']; ?>"> 
				</div>
			</div> <!-- .form-group -->

			<div class="form-group row">
				<label for="rating-id" class="col-sm-3 col-form-label text-sm-right">
					Release Date:
				</label>
				<div class="col-sm-9">
					<input type="date" class="form-control mt-2" name="release_date" value="<?php echo $row_dvd['release_date']; ?>">
				</div>
			</div> <!-- .form-group -->

			<input type="hidden" name="dvd_title_id" value="
				<?php echo $_GET['dvd_title_id']; ?>">

			<div class="form-group row">
				<div class="ml-auto col-sm-9">
					<span class="text-danger font-italic">* Required</span>
				</div>
			</div> <!-- .form-group -->

			<div class="form-group row">
				<div class="col-sm-3"></div>
				<div class="col-sm-9 mt-2">
					<button type="submit" class="btn btn-primary">Submit</button>
					<button type="reset" class="btn btn-light">Reset</button>
				</div>
			</div> <!-- .form-group -->
		</form>
	</div> <!-- .container -->
</body>
</html>