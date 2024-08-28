## 2.1 Naming Convention

- **Avoid Special Characters**: Do not use special characters such as ~, !, @, #, $, %, ^, &, *, `, ;, :, <, >, ?, ., ,, ', ", |, ö, ä, ü.
- **No Spaces**: Do not include spaces in file names; use alphanumeric characters, dashes, or underscores.
- **Keep It Short**: Keep names as concise as possible while conveying the asset's function in the game.
- **CamelCase**: Utilize CamelCase naming conventions for clarity.
- **English Only**: Only use English names (no mix between German and English)
- **Functional Naming**: Name assets based on their intended function in the game, avoiding vague or redundant descriptions.

## 2.2 Consistency

Maintain consistency across all file names and metadata. Apply the same information to all relevant files.

## 2.3 Avoid Redundant Information

When naming files, avoid repeating information already present in the file structure. For instance, if an asset is located at "1_Assets/4_Gamescreens/6_UI/4_Scorebar," there is no need to include all these details in the file name. Instead, name the file "mask.png," as the filepath already provides the necessary context.

## 2.5 Content Based File System

The file system is content-based rather than file type-based.

This means that files are not sorted solely by their type in different folders. Instead, they are organized based on their purpose or usage in the game. For example, if it's a UI element in the game screen, it will be found in the folder ../Contents/2_Game/2_UI/4_OxygenContainer.

## 2.6 Handling Complex Issues And Edgecases

In cases where issues become complex, you may need to schedule a call with someone from the engine team for precise specifications.

## 2.7 Provide Detailed Asset Usage Specifications

- When submitting an asset, it's crucial to provide precise specifications on how the asset is intended to be used and its expected behavior within the game.
- Include details such as when the asset should appear, its appearance on mouse hover, exact position, and any other relevant information.
- Be as specific and detailed as possible in your communication with the Engine Team to ensure accurate asset integration.
- You always have the possibility to use a ReadMe files in the assets folder to specify usage (only use .txt files for that)

## 2.8 Requirements For User Interface Assets

- **Nine-Slicing Requirement**: Every asset must be nine-slicable. Exceptions require discussion with the engine team.
- **Size for Nine-Slicables**: Use the smallest dimensions possible for them.
- **Size Guidelines for Non-Slicables**: For non-slicable assets, their dimensions must be a power of 2.
- **Anti-Aliasing Restrictions**: Avoid using anti-aliasing on assets unless specified by the engine team.
- **File Format**: .png unless specified differently