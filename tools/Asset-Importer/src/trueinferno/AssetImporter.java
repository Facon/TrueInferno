package trueinferno;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Properties;

import org.apache.commons.io.FileUtils;

import trueinferno.ImportMaterialTask.MaterialTaskOption;
import trueinferno.ImportMeshTask.MeshTaskOption;

public class AssetImporter {

	/** Ruta del fichero de alias de nombres de edificios */
	private static final String BUILDING_ALIAS_PATH = "resources/buildingAlias.properties";

	/** Ruta del fichero de configuración de tareas */
	private static final String TASK_CONFIG_PATH = "resources/task.properties";

	private Properties taskConfigProperties;
	
	/** Carpeta root del proyecto C++ */
	private File projectRoot;
	
	/** Carpeta root de los assets de entrada */
	private File inputAssetRoot;
	
	/** Carpeta de los modelos de entrada */
	private File inputModelDir;
	
	/** Mapa de alias de nombres de edificios: <nombre en el juego>-<nombre en la carpeta de assets> */
	private BuildingNameAtlas buildingNameAtlas;

	/** Fichero para reportar el resultado del import */
	private FileWriter reportFile;

	/** Carpeta de salida de Galeón donde se escribirán o modificarán los modelos */
	private File outputModelDir;

	/** Carpeta de salida de Galeón donde se escribirán o modificarán los materiales */
	private File outputBuildingMaterialDir;

	/** Carpeta de salida de Galeón donde se escribirán o modificarán las texturas */
	private File outputBuildingTextureDir;
	
	/** Ruta del fichero de mapa de Galeón */
	private File mapFile;

	public AssetImporter(File projectRoot, File assetRoot, File reportFile) throws IOException {
		this.projectRoot = projectRoot;
		this.inputAssetRoot = assetRoot;
		this.reportFile = new FileWriter(reportFile, false);
		buildingNameAtlas = new BuildingNameAtlas();
		taskConfigProperties = new Properties();
	}
	
	private void init() throws TrueInfernoException, IOException {
		if(!isProjectRoot(projectRoot))
			throw new TrueInfernoException("'"+projectRoot+"' is not a valid project root ('Exes' and/or 'Projects' subfolder not found inside)");
		
		if(!isInputAssetRoot(inputAssetRoot))
			throw new TrueInfernoException("'"+inputAssetRoot+"' is not a valid input asset root ('Modelos3D' subfolder not found inside)");
		
		inputModelDir = FileUtils.getFile(inputAssetRoot, "Modelos3D");
		if(!inputModelDir.exists() || !inputModelDir.isDirectory())
			throw new TrueInfernoException("Folder '"+inputModelDir+"' doesn't exist");
		
		outputModelDir = FileUtils.getFile(projectRoot, "Exes", "media","models");
		if(!outputModelDir.exists() || !outputModelDir.isDirectory())
			throw new TrueInfernoException("Folder '"+outputModelDir+"' doesn't exist");
		
		outputBuildingMaterialDir = FileUtils.getFile(projectRoot, "Exes", "media","materials", "scripts");
		if(!outputBuildingMaterialDir.exists() || !outputBuildingMaterialDir.isDirectory())
			throw new TrueInfernoException("Folder '"+outputBuildingMaterialDir+"' doesn't exist");
		
		outputBuildingTextureDir = FileUtils.getFile(projectRoot, "Exes", "media","materials", "textures", "building");
		if(!outputBuildingTextureDir.exists() || !outputBuildingTextureDir.isDirectory())
			throw new TrueInfernoException("Folder '"+outputBuildingTextureDir+"' doesn't exist");
		
		mapFile = FileUtils.getFile(projectRoot, "Exes", "media", "maps", "map.txt");
		if(!mapFile.exists())
			throw new TrueInfernoException("File '"+mapFile+"' doesn't exist");
		
		loadBuildingAlias(new File(BUILDING_ALIAS_PATH));
		
		taskConfigProperties.load(new FileInputStream(new File(TASK_CONFIG_PATH)));
		
		printReportHeader();
	}


	private void loadBuildingAlias(File file) throws FileNotFoundException, IOException {
		Properties properties = new Properties();
		properties.load(new FileInputStream(file));
		buildingNameAtlas.load(properties);
	}
	
	/** 
	 * Comprueba si un directorio tiene pinta de ser el root del proyecto C++ según sus subdirectorios
	 * @param dir
	 * @return true/false si el directorio tiene lo esperado o no 
	 * @throws IOException
	 */
	private boolean isProjectRoot(File dir) throws IOException {
		return 
				FileUtils.directoryContains(dir, new File(dir.getAbsolutePath() + File.separator + "Exes")) && 
				FileUtils.directoryContains(dir, new File(dir.getAbsolutePath() + File.separator + "Projects"));
	}

	/** 
	 * Comprueba si un directorio tiene pinta de ser el root de la carpeta de assets según sus subdirectorios
	 * @param dir
	 * @return true/false si el directorio tiene lo esperado o no 
	 * @throws IOException
	 */
	private boolean isInputAssetRoot(File dir) throws IOException {
		return 
				FileUtils.directoryContains(dir, new File(dir.getAbsolutePath() + File.separator + "Modelos3D"));
	}
	
	/**
	 * Importa los assets
	 * @throws TrueInfernoException
	 * @throws IOException 
	 */
	public void importAssets() throws TrueInfernoException, IOException {
		try {
			init();
		} catch (Exception e) {
			throw new TrueInfernoException("Error initializing import", e);
		}
		
		try {
			importBuildings();	
		} catch (Exception e) {
			throw new TrueInfernoException("Error importing buildings", e);
		}
		
		close();
	}

	private void close() throws IOException {
		reportFile.close();
	}

	private void importBuildings() throws TrueInfernoException, IOException {
		// Para cada nombre real de edificio del juego
		for(String buildingRealName : buildingNameAtlas.getRealNames()){
			importBuilding(buildingRealName);
		}
	}

	private void importBuilding(String realName) throws TrueInfernoException, IOException {
		String givenName = buildingNameAtlas.realName2GivenName(realName);
		
		MeshTaskOption meshTaskOption = MeshTaskOption.valueOf(taskConfigProperties.getProperty("mesh"));
		if(meshTaskOption != MeshTaskOption.SKIP){
			Task task = new ImportMeshTask(realName, givenName, inputModelDir, outputModelDir, mapFile, meshTaskOption);
			task.execute();
			report(task);
		}
			
		MaterialTaskOption materialTaskOption = MaterialTaskOption.valueOf(taskConfigProperties.getProperty("material"));
		if(materialTaskOption != MaterialTaskOption.SKIP){
			Task task = new ImportMaterialTask(realName, givenName, inputModelDir, outputBuildingMaterialDir, outputBuildingTextureDir, mapFile, materialTaskOption);
			task.execute();
			report(task);
		}
	}


	private void printReportHeader() throws IOException {
		reportFile.append("title" + "\t" + "description" + "\t" + "result" + "\t" + "observations" + "\n");
	}

	private void report(Task task) throws IOException {
		reportFile.append(task.getTitle() + "\t" + task.getDescription() + "\t" + task.getResult() + "\t" + task.getObservations() + "\n");
	}
	
	
}
