# V59 bounded source compatibility review

V59 supersedes V58's proposed owner-setting binder. Frozen V58 source/archive are not modified. Source reads plus unique successor writes only; no compiler, tests, oracle/native provider, GPU/decoder work.

Concrete ownership mismatch corrected: existing MainEncodedImageBudgetProviderV46::image_services produces MaterialImageServicesV43 `{self,resolver,[self]read}`. V58 declared a binder setting out.canonical_uri AND out.owner, replacing that retained provider owner. MaterialImageReadV43.resource_owner later copies this exact owner. V59 returns ONLY `decltype(MaterialImageServicesV43::canonical_uri)` and lets Main's existing V46 image_services install it. Existing owner/read and V45 admission pin remain untouched. Resolver callback captures genuine Scene/CRes/filesystem pins and weak/current renderer checks, never a copied ActorVisualReadV37/draw reference after callback return.

Source-compatible invocation shape (pseudocode inside Main's current guarded borrow scope, not compiled/provider implementation):

```cpp
MainCanonicalImageResolverV59 actual_resolver;
if(!make_main_full_simage_canonical_uri_v59(actual_actor_read,actual_provider,actual_resolver,error))return false;
if(!actual_v46_provider->image_services(actual_resolver,owner.images,error))return false;
// V45.admit_before_read remains bound to SAME actual_v46_provider.
```

Exact current callback is bool(const MaterialImageIdentityV43&,std::string&,std::string&). V59 derives its type directly from V43, avoiding a second ABI spelling. V45 borrow_resources already receives const ActorVisualReadV37& and ActorImageOwnerBorrowV45& output. V45 consumer already receives const ActorPrimitiveImagesReadV45& and std::string&; its images vector holds ActorMaterialImageBindingV45, including MaterialImageReadV43 and genuine CPU admission pin. V59 sampler consumer uses these types unchanged. Main V50 current-renderer predicate is bool(std::string&); V59 CRes-scope predicate is an additive scoped adapter declaration, not a function-pointer cast or claim V50 already supplies CRes identity/generation. Main must bind its actual current-owner predicate inside the adapter. No actual CRes provider is implemented by this packet.

Main decoder/upload APIs remain unchanged. ResourceKindV37 currently has8valid kinds(texture,vertex_buffer,index_buffer,program,framebuffer,renderbuffer,cpu_request,shader) plus count sentinel. ResourceScopeV37 has10valid scopes plus count sentinel. Prior enum9 counted entries including sentinel. V59 contains no budget enum cast, numeric kind/scope, budget counter/array or second budget owner; no enum layout crosses this seam. Budget operations stay inside actual V46 and Main decoded/GPU providers.

No other concrete callback-type mismatch found in these declarations. This is source review, not syntax/link/runtime proof. Missing authentic filesystem/format/embedded-image selector, Scene/CRes borrow and technique/backend providers remain explicit. No pvr2 alias/resolution fabricated.
